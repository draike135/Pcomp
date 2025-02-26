#include <gd.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "image-lib.h"

#define OLD_IMAGE_DIR "/old_photo_PAR_B/"
int pipe_fd[2];
int pipe_fdInput[2];
pthread_mutex_t mutex;

typedef struct filenames{
    char *filename;
    struct filenames *next;
    struct timespec time;

} filenames;

typedef struct thread_arg_t{
    gdImagePtr texture_img;
    FILE *fptr;
    char *directory;
    struct thread_arg_t *next;
    int threadid;
    int processed_images;
    filenames *files;
    int num_files;
} thread_arg_t;


/******************************************************************************
 * oldphoto()
 *
 * Arguments:
 *    in_img - pointer to the input image
 *    in_texture_img - pointer to the texture image
 *
 * Returns:
 *    gdImagePtr - pointer to the final processed image (sepia-toned)
 *
 * Side-Effects:
 *    Frees intermediate images (contrast, smoothed, and textured)
 *
 * Description:
 *    Applies a series of image processing effects to create an "old photo" effect.
 *
 *****************************************************************************/
gdImagePtr oldphoto(gdImagePtr in_img, gdImagePtr in_texture_img) {
    gdImagePtr out_contrast_img = contrast_image(in_img);
    gdImagePtr out_smoothed_img = smooth_image(out_contrast_img);
    gdImagePtr out_textured_img = texture_image(out_smoothed_img, in_texture_img);
    gdImagePtr out_sepia_img = sepia_image(out_textured_img);

    gdImageDestroy(out_contrast_img);
    gdImageDestroy(out_smoothed_img);
    gdImageDestroy(out_textured_img);

    return out_sepia_img;
}
/******************************************************************************
 * free_string_array2()
 *
 * Arguments:
 *    array - double pointer to an array of strings (char **)
 *    size - number of elements in the array
 *
 * Returns:
 *    None
 *
 * Side-Effects:
 *    Frees all dynamically allocated memory for the array of strings and the
 *    array itself.
 *
 * Description:
 *    Safely frees a dynamically allocated array of strings.
 *
 *****************************************************************************/
void free_string_array2(char **array, int size) {
    if (array == NULL) return; // Ensure the array is not NULL

    for (int i = 0; i < size; i++){
        if (array[i] != NULL) {
            free(array[i]); // Free each string
        }
    }

    free(array); // Free the array itself
}
/******************************************************************************
 * freeThreadArgs()
 *
 * Arguments:
 *    head - pointer to the head of the linked list of thread_arg_t structures
 *
 * Returns:
 *    None
 *
 * Side-Effects:
 *    Frees all dynamically allocated memory for the linked list, including
 *    each node, associated file arrays, and directory strings.
 *
 * Description:
 *    Iterates through the linked list of thread_arg_t structures, freeing
 *    memory for each node and its associated resources (files and directory).
 *
 *****************************************************************************/
void freeThreadArgs(thread_arg_t *head){
    thread_arg_t *current = head;  // Start at the head
    thread_arg_t *next;
    while (current != NULL) {
        next = current->next; // Save the next node
        while(current->files != NULL){
            filenames *aux = current->files;
            current->files = current->files->next;
            if(aux->filename != NULL){
                free(aux->filename);
            }

            free(aux);
        }
        if (current->directory != NULL) {
            free(current->directory); // Free the directory string
        }
        free(current); // Free the node
        current = next; // Move to the next node
    }

}
/******************************************************************************
 * process_photos()
 *
 * Arguments:
 *    arg - pointer to thread_arg_t structure containing thread-specific data
 *
 * Returns:
 *    NULL
 *
 * Side-Effects:
 *    Processes images by applying an "old photo" effect and saves them to an
 *    output directory. Logs processing time for the thread to a file.
 *
 * Description:
 *    This function, designed to run as a thread, iterates through a list of
 *    input image files, checks if the output image already exists, and if not:
 *      1. Reads the input JPEG file.
 *      2. Applies the "old photo" effect.
 *      3. Writes the processed image to the output directory.
 *    Finally, it calculates the total processing time for the thread and
 *    writes it to a log file.
 *
 *****************************************************************************/
void *process_photos(void * arg) {
    char *buffer;
    thread_arg_t *targ = (thread_arg_t *)arg;
    char *out_file_name,*bufferWrite;
    char filename[100];
    struct timespec startimethread,endtimethread,starttimefoto,endtimefoto;
    clock_gettime(CLOCK_MONOTONIC, &startimethread);
    filenames *current;
    filenames *aux = (filenames*)malloc(sizeof(filenames));

    while(1){
        if (read(pipe_fd[0],filename,100) <= 0)
        {
            break;
        }
        aux->filename = (char*)malloc((strlen(filename)+1)*sizeof(char));
        strcpy(aux->filename,filename);
        out_file_name = (char*)malloc((strlen(targ->directory)+strlen(OLD_IMAGE_DIR)+strlen(filename)+1)*sizeof(char));
        sprintf(out_file_name, "%s%s%s",targ->directory,OLD_IMAGE_DIR,filename);
        clock_gettime(CLOCK_MONOTONIC, &starttimefoto);
        if(access( out_file_name, F_OK ) != -1){
        }else{
            buffer = (char*)malloc((strlen(targ->directory)+strlen(filename)+1)*sizeof(char));
            sprintf(buffer,"%s/%s",targ->directory,filename);

            gdImagePtr in_img = read_jpeg_file(buffer);
            if (in_img == NULL) {
                fprintf(stderr, "Error reading file %s\n", buffer);
                continue;
            }
            free(buffer);
            gdImagePtr out_sepia_img = oldphoto(in_img, targ->texture_img);


            if (write_jpeg_file(out_sepia_img, out_file_name) == 0) {
                fprintf(stderr, "Error writing file %s\n", out_file_name);
            }
            gdImageDestroy(in_img);
            gdImageDestroy(out_sepia_img);
        }
        free(out_file_name);
        clock_gettime(CLOCK_MONOTONIC, &endtimefoto);
        struct timespec timefoto = diff_timespec(&endtimefoto, &starttimefoto);
        aux->time = timefoto;

        if(targ->processed_images == 0){
            targ->files = aux;
            aux = (filenames*)malloc(sizeof(filenames));
            if(aux == NULL){
                fprintf(stderr, "Error allocating memory for filenames\n");
                exit(0);
            }
            current = targ->files;
            current->next = NULL;
        }else{

        current->next = aux;
        current = current->next;
        current->next = NULL;
        aux = (filenames*)malloc(sizeof(filenames));
        if(aux == NULL){
            fprintf(stderr, "Error allocating memory for filenames\n");
            exit(0);
        }
        }
        targ->processed_images++;
    }
    free(aux);

    clock_gettime(CLOCK_MONOTONIC, &endtimethread);
    struct timespec thread_total_time = diff_timespec(&endtimethread, &startimethread);
    bufferWrite = (char*)malloc(64*sizeof(char));
    sprintf(bufferWrite,"Thread %d:\t %10jd.%09ld\n",targ->threadid, thread_total_time.tv_sec, thread_total_time.tv_nsec);
    fprintf(targ->fptr,"%s",bufferWrite);
    free(bufferWrite);
    return NULL;
}
/******************************************************************************
 * input_reader()
 *
 * Arguments:
 *    arg - pointer to thread_arg_t structure containing thread-specific data
 *
 * Returns:
 *    NULL
 *
 * Side-Effects:
 *    Reads input from the user and processes it.
 *
 * Description:
 *    This function, designed to run as a thread, reads input from the user and
 *    processes it. If the input is "s", the program stops processing images.
 *    Otherwise, it prints an error message.
 *
 ****************************************************************************/
 void *input_reader(void *arg){

     thread_arg_t *targ = (thread_arg_t *)arg;
     char text[100] = {0};
     char *newline;
     int totalImages = 0;
     fd_set rfds,rfdsaux;
     while(1){
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        FD_SET(pipe_fdInput[0], &rfds);
        rfdsaux = rfds;
        if(select(pipe_fdInput[0]+1, &rfdsaux, NULL, NULL, NULL) < 0){
            fprintf(stderr, "Error selecting\n");
            return NULL;
        }
        if(FD_ISSET(pipe_fdInput[0], &rfdsaux)){
            break;
        }else{
        if (read(0,text,100) <= 0) {
            break; // Exit on EOF or error
        }
         newline = strchr(text, '\n');

         if (newline != NULL) {
             *newline = '\0'; // Replace newline with null terminator
         }


         if (strcmp(text,"s")==0)
         {   totalImages = 0;
            pthread_mutex_lock(&mutex);
            for(thread_arg_t *current = targ; current != NULL; current = current->next)
             {
                 for (filenames *current2 = current->files; current2 != NULL; current2 = current2->next)
                 {
                     if(current2->time.tv_sec!=0 || current2->time.tv_nsec!=0 || current2->filename!=NULL || current2->time.tv_sec>10000){
                         printf("%s\t%jd.%09ld\n",current2->filename, current2->time.tv_sec, current2->time.tv_nsec);
                         totalImages++;
                         }
                 }
                 printf("Thread %d processed %d images\n",current->threadid,current->processed_images);

             }
             printf("There are %d images left\n",targ->num_files-totalImages);
            pthread_mutex_unlock(&mutex);
         }else if(strcmp(text,"£")==0){

             return NULL;

         }
         else{
         }
        }

     }

     return NULL;

 }
/******************************************************************************
 * old_photo_final()
 *
 * Arguments:
 *    files - array of file names to process
 *    nn_files - total number of files
 *    num_threads - number of threads to use
 *    dir - directory where the files are located
 *    fptr - file pointer for logging
 *
 * Returns:
 *    int - 0 on success, -1 on error
 *
 * Side-Effects:
 *    - Creates an output directory for processed images.
 *    - Allocates memory for thread arguments and divides work among threads.
 *    - Spawns threads to process images and waits for completion.
 *    - Frees all allocated memory and resources.
 *
 * Description:
 *    This function applies the "old photo" effect to a set of image files
 *    using a multithreaded approach. It performs the following steps:
 *      1. Creates an output directory for the processed images.
 *      2. Loads a texture image used for the effect.
 *      3. Allocates and initializes thread arguments.
 *      4. Divides the work (image files) among threads.
 *      5. Spawns threads to process images concurrently.
 *      6. Waits for all threads to complete and logs the total processing time.
 *      7. Frees all allocated memory and destroys the texture image.
 *
 *****************************************************************************/

int old_photo_final(char **files, int nn_files,  int num_threads,char *dir, FILE *fptr) {
    // Create output directory
    struct timespec startimeNP,endtimeNP;
    clock_gettime(CLOCK_MONOTONIC, &startimeNP);

    char *buffer,*fileinfo;
    buffer =  (char*)malloc((strlen(dir)+strlen(OLD_IMAGE_DIR)+1)*sizeof(char));
    if(buffer == NULL){
        fprintf(stderr, "Error creating output directory\n");
        return -1;
    }
    sprintf(buffer,"%s%s",dir,OLD_IMAGE_DIR);

    if (create_directory(buffer) == 0){
        free(buffer);
        fprintf(stderr, "Error creating output directory\n");
        return -1;
    }
    free(buffer);
    // Load texture image
    gdImagePtr in_texture_img = read_png_file("./paper-texture.png");
    if (in_texture_img == NULL) {
        fprintf(stderr, "Error loading texture image\n");
        return -1;
    }

    if (pipe(pipe_fd) != 0) {
        gdImageDestroy(in_texture_img);
        fprintf(stderr, "Failed to create pipe\n");
        return -1;
    }

    
    if(pipe(pipe_fdInput) != 0){
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        gdImageDestroy(in_texture_img);
        fprintf(stderr, "Failed to create pipe\n");
        return -1;
    }

    if(pthread_mutex_init(&mutex, NULL) != 0){
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(pipe_fdInput[0]);
        close(pipe_fdInput[1]);
        gdImageDestroy(in_texture_img);
        fprintf(stderr, "Failed to create mutex\n");
        return -1;
    }
    pthread_t threads[num_threads+1];

    thread_arg_t *current,*threadInfo = NULL;
    thread_arg_t *aux = (thread_arg_t*)malloc(sizeof(thread_arg_t));
    if(aux == NULL){
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(pipe_fdInput[0]);
        close(pipe_fdInput[1]);
        gdImageDestroy(in_texture_img);
        pthread_mutex_destroy(&mutex);
        fprintf(stderr, "Error allocating memory for thread arguments\n");
        return -1;
    }

    for (int i = 0; i < num_threads; i++)
    {
        aux->fptr = fptr;
        aux->texture_img = in_texture_img;
        aux->num_files = nn_files;
        aux->files = NULL;
        aux->directory = (char*)malloc((strlen(dir)+1)*sizeof(char));
        if(aux->directory == NULL){
            
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            close(pipe_fdInput[0]);
            close(pipe_fdInput[1]);
            gdImageDestroy(in_texture_img);
            if (threadInfo != NULL)
            {
                freeThreadArgs(threadInfo);
            }else{
                free(aux->directory);
                free(aux);
            }
        
            pthread_mutex_destroy(&mutex);
            fprintf(stderr, "Error allocating memory for directory\n");
            return -1;
        }
        strcpy(aux->directory,dir);
        aux->threadid = i;
        aux->processed_images = 0;
        if(i == 0){
            threadInfo = aux;
            current = threadInfo;
        }else{
            current->next = aux;
            current = current->next;
        }
        aux = (thread_arg_t*)malloc(sizeof(thread_arg_t));
        if(aux == NULL){
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            close(pipe_fdInput[0]);
            close(pipe_fdInput[1]);
            gdImageDestroy(in_texture_img);
            freeThreadArgs(threadInfo);
            pthread_mutex_destroy(&mutex);
            fprintf(stderr, "Error allocating memory for thread arguments\n");
            return -1;
        }
    }
    current->next = NULL;
    free(aux);
    for (thread_arg_t *current = threadInfo; current != NULL; current = current->next)
    {   
        if (pthread_create(&threads[current->threadid], NULL, process_photos, current) != 0){

            close(pipe_fd[0]);
            close(pipe_fd[1]);
            close(pipe_fdInput[0]);
            close(pipe_fdInput[1]);
            gdImageDestroy(in_texture_img);
            freeThreadArgs(threadInfo);
            fprintf(stderr, "Failed to create thread\n");

            return -1;
        }
        
    }

    if (pthread_create(&threads[num_threads], NULL, input_reader, threadInfo) != 0) {
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        close(pipe_fdInput[0]);
        close(pipe_fdInput[1]);
        gdImageDestroy(in_texture_img);
        fprintf(stderr, "Failed to create thread\n");
        freeThreadArgs(threadInfo);
        return -1;
    }

    for (int i = 0; i < nn_files; i++)
    {
        fileinfo = (char*)malloc((strlen(files[i])+1)*sizeof(char));
        if(fileinfo == NULL){
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            close(pipe_fdInput[0]);
            close(pipe_fdInput[1]);
            gdImageDestroy(in_texture_img);
            freeThreadArgs(threadInfo);
            fprintf(stderr, "Error allocating memory for fileinfo\n");
            return -1;
        }
        sprintf(fileinfo,"%s",files[i]);
        write(pipe_fd[1], fileinfo, 100);
        free(fileinfo);
    }
    close(pipe_fd[1]);
    for (int i = 0; i < num_threads; i++)
    {
        if(pthread_join(threads[i], NULL) != 0){
            close(pipe_fd[0]);
            freeThreadArgs(threadInfo);
            fprintf(stderr, "Failed to join thread\n");
            return -1;
        }
    }

    close(pipe_fd[0]);
    close(pipe_fdInput[1]);
    close(pipe_fdInput[0]);
    pthread_join(threads[num_threads],NULL);
    pthread_mutex_destroy(&mutex);
    clock_gettime(CLOCK_MONOTONIC, &endtimeNP);
    printf("All threads finished\n");
    struct timespec timeNP = diff_timespec(&endtimeNP, &startimeNP);
    buffer = (char*)malloc(59*sizeof(char));
    if(buffer == NULL){
        freeThreadArgs(threadInfo);
        gdImageDestroy(in_texture_img);
        fprintf(stderr, "Error allocating memory for buffer\n");
        return -1;
    }
    sprintf(buffer,"OldPhotoTotal:\t %6jd.%09ld\n", timeNP.tv_sec, timeNP.tv_nsec);
    fprintf(fptr,"%s",buffer);
    free(buffer);
    gdImageDestroy(in_texture_img);
    freeThreadArgs(threadInfo);
    return 0;
}

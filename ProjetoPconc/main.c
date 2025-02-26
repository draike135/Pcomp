#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>

#include "image-lib.h"
#include "old-photo-serial.h"
#include "Dynamic.h"
#include "Handler.h"
#include "sort.h"
/******************************************************************************
 * main()
 *
 * Arguments: argc - number of command-line arguments
 *            argv - array of command-line arguments
 *
 * Returns: 0 on success
 * Side-Effects: Writes output to a file and allocates/frees memory.
 *
 * Description: The main program workflow: validates arguments, reads directory
 *              files into a list, sorts (if needed), and processes files.
 *
 *****************************************************************************/
 int main(int argc, char *argv[]) {

     struct timespec start_time_total, end_time_total,start_time_NonP,end_time_NonP;
     clock_gettime(CLOCK_MONOTONIC, &start_time_total);
     clock_gettime(CLOCK_MONOTONIC, &start_time_NonP);

     Lista *head, *current;
     FILE *fptr;
     int n_threads, n_of_files = 0, flag = 0,t=0;
     char **files;
     char *buffer;


     checkArgs(argc,argv);
     printf("Arguments are correct\n");
     fptr = fileHandling(argv);
     printf("File created\n");
     head = Directory(argv);

     if(head == NULL){
         fprintf(stderr, "Error reading directory\n");
         fclose(fptr);
         exit(0);
     }

     printf("Directory read\n");
     n_threads = atoi(argv[2]);
     current = head;
     while(current!=NULL){
         n_of_files++;
         current = current->next;
     }

     if (n_threads > 0 && n_threads <= n_of_files)
     {
         if (strcmp("-size",argv[3])==0)
         {
             flag = 1;
         }
         head = quickSort(head,flag);
         printf("List sorted\n");
         files = fileNames(head,n_of_files);
         if (files == NULL)
         {
            fclose(fptr);
            freeListas(head);
            exit(0);
         }
         
         printf("File names extracted\n");
         current = head;
         while(current!=NULL){
             t++;
             current = current->next;
         }
         printf("\nNumber of files: %d\nNumber of threads: %d\nDirectory: %s\n",n_of_files,n_threads,argv[1]);
         clock_gettime(CLOCK_MONOTONIC, &end_time_NonP);
         if(old_photo_final(files,n_of_files,n_threads, argv[1], fptr)==-1){
             fprintf(stderr, "Error processing images\n");
             freeListas(head);
             free_string_array(files,n_of_files);
             fclose(fptr);
             exit(-1);
         }

         free_string_array(files,n_of_files);

     }
     freeListas(head);

     clock_gettime(CLOCK_MONOTONIC, &end_time_total);
     struct timespec total_time = diff_timespec(&end_time_total, &start_time_total);
     struct timespec total_timeNP = diff_timespec(&end_time_NonP, &start_time_NonP);
     int size;
     size = snprintf(NULL, 0, "Tempo NP:\t %10jd.%09ld\n", total_timeNP.tv_sec, total_timeNP.tv_nsec);
     buffer = (char*)malloc((size + 1) * sizeof(char));  // +1 for null terminator
     if (buffer == NULL) {
         fprintf(stderr, "Error allocating memory for buffer\n");
         fclose(fptr);
         exit(0);
     }

     // Write the formatted string into the buffer and print to file
     sprintf(buffer, "Tempo NP:\t %10jd.%09ld\n", total_timeNP.tv_sec, total_timeNP.tv_nsec);
     fprintf(fptr, "%s", buffer);
     free(buffer);

     // Calculate the size needed for the second formatted string
     size = snprintf(NULL, 0, "Tempo Total:\t %6jd.%09ld\n", total_time.tv_sec, total_time.tv_nsec);
     buffer = (char*)malloc((size + 1) * sizeof(char));  // +1 for null terminator
     if (buffer == NULL) {
         fprintf(stderr, "Error allocating memory for buffer\n");
         fclose(fptr);
         exit(0);
     }

     // Write the second formatted string into the buffer and print to file
     sprintf(buffer, "Tempo Total:\t %6jd.%09ld\n", total_time.tv_sec, total_time.tv_nsec);
     fprintf(fptr, "%s", buffer);
     free(buffer);

     fclose(fptr);

     return 0;
 }

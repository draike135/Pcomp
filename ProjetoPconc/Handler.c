#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <sys/types.h>
#include "Handler.h"
#include "Dynamic.h"
//Done
/******************************************************************************
 * get_file_extension()
 *
 * Arguments: filename - string containing the filename
 *
 * Returns: Pointer to the file extension string or "Error" if not found.
 * Side-Effects: none
 *
 * Description: Extracts and returns the file extension from a filename string.
 *
 *****************************************************************************/
char *get_file_extension(char *filename) {

    char *dot = strrchr(filename, '.'); //get file extension after '.'

    if (!dot || dot == filename || *(dot + 1) == '\0') {
        return "Error";
    }

    return dot + 1; //return everything after the last .
}
//Done
/******************************************************************************
 * checkArgs()
 *
 * Arguments: argc - number of command-line arguments
 *            argv - array of command-line arguments
 *
 * Returns: none
 * Side-Effects: Exits the program if arguments are invalid.
 *
 * Description: Validates the command-line arguments for correct format and
 *              structure.
 *
 *****************************************************************************/
void checkArgs(int argc, char *argv[]){

    if((argc-1) != 3) //Wrong number of arguments
    {

        fprintf(stderr, "Invalid number of arguments.\n");
        exit(0);
    }

    int warningAux = strlen(argv[2]);

    if (!(argv[1][0]== '.' || argv[1][0]== '/')) //check if starts with '/' or '.'
    {
        fprintf(stderr, "Invalid directory.\n");
        exit(0);
    }

    for (int i = 0; i < warningAux; i++) //see if its a valid number
    {
        if (argv[2][i] < 48 || argv[2][i] > 57)
        {
            fprintf(stderr, "Invalid number of threads.\n");
            exit(0);
        }
    }

    if (!(strcmp("-name", argv[3])==0||strcmp("-size", argv[3])==0)) //see if its -size or -name
    {
        fprintf(stderr, "Invalid sort type.\n");
        exit(0);
    }
}
/******************************************************************************
 * fileHandling()
 *
 * Arguments: argv - array of command-line arguments
 *
 * Returns: Pointer to a newly opened file
 * Side-Effects: Opens a file for writing
 *
 * Description: Creates and opens a timing file based on the input directory,
 *              thread count, and sort type (-name or -size).
 *
 *****************************************************************************/
FILE *fileHandling(char *argv[]){
    FILE *fptr;
    char *bufferFile;
    bufferFile = (char*)malloc(15 + strlen(argv[1])+strlen(argv[2])+strlen(argv[3])*sizeof(char));
    if (bufferFile == NULL)
    {
        fprintf(stderr, "Error allocating memory for file name\n");
        exit(0);
    }

    sprintf(bufferFile,"%s/timing_B_%d%s.txt",argv[1],atoi(argv[2]),argv[3]);
    fptr = fopen(bufferFile,"w");//create file in directory
    free(bufferFile);
    return fptr;

}

/******************************************************************************
 * Directory()
 *
 * Arguments: argv - array of command-line arguments
 *
 * Returns: Pointer to the head of the LinkedList
 * Side-Effects: Allocates memory for a LinkedList.
 *
 * Description: Reads files from a directory, filters JPEG files, and creates
 *              a LinkedList with their names and sizes.
 *
 *****************************************************************************/
 Lista *Directory(char *argv[]) {
     char *buffer, *buffer2;
     DIR *d;
     struct dirent *f;
     struct stat st;
     Lista *head, *current;

     // Allocate memory for the head of the list
     head = (Lista *)malloc(sizeof(Lista));
     if (head == NULL) {
         fprintf(stderr, "Failed to allocate memory for the list\n");
         return NULL;
     }

     // Initialize head
     head->word = NULL;
     head->size = 0.0;
     head->next = NULL;

     // Attempt to open the directory
     d = opendir(argv[1]);
     if (d) {
         f = readdir(d);
         if (f == NULL) {
             fprintf(stderr, "Failed to read directory\n");
             free(head);
             closedir(d);
             return NULL;
         }

         stat(f->d_name, &st);

         current = head;

         // Allocate buffer to store the directory path
         buffer = (char *)malloc((strlen(argv[1]) + 2) * sizeof(char));
         if (buffer == NULL) {
             fprintf(stderr, "Failed to allocate memory for the buffer\n");
             free(head);
             closedir(d);
             return NULL;
         }
         strcpy(buffer, argv[1]);

         // Loop through all files in the directory
         while ((f = readdir(d)) != NULL) {

            // Check if the file has a .jpeg extension
            if (strcmp("jpeg", get_file_extension(f->d_name)) == 0) {
                // Allocate memory for the file name
                current->word = (char *)malloc((strlen(f->d_name) + 1) * sizeof(char));
                if (current->word == NULL) {
                    fprintf(stderr, "Failed to allocate memory for the file name\n");
                    freeListas(head);
                    closedir(d);
                    free(buffer);
                    return NULL;
                }
                strcpy(current->word, f->d_name);
                buffer2 = (char *)malloc((strlen(buffer) + strlen(f->d_name) + 2) * sizeof(char));
                if (buffer2 == NULL) {
                    fprintf(stderr, "Failed to allocate memory for the buffer\n");
                    freeListas(head);
                    closedir(d);
                    free(buffer);
                    return NULL;
                }

                // Create the full file path
                sprintf(buffer2, "%s/%s", buffer, f->d_name);

                // Check if the file exists and get its size
                if (stat(buffer2, &st) == 0) {
                    current->size = st.st_size;
                } else {
                    fprintf(stderr, "Failed to retrieve file information for %s\n", buffer2);
                    current->size = 0;
                }
                free(buffer2);

                // Allocate and initialize the next node
                current->next = (Lista *)malloc(sizeof(Lista));
                if (current->next == NULL) {
                    fprintf(stderr, "Failed to allocate memory for the next node\n");
                    freeListas(head);
                    closedir(d);
                    free(buffer);
                    return NULL;
                }
                current = current->next;  // Move to the next node
                current->word = NULL;      // Initialize new node
                current->size = 0.0;
                current->next = NULL;
            }
    }
         for (Lista *i = head; i->next != NULL; i = i->next) {
             if (i->next->next == NULL) {
                 i->next = NULL;
                 break;
             }
         }
 

         free(buffer);


         free(current);

         closedir(d);

     } else {
         fprintf(stderr, "Failed to open directory\n");
         closedir(d);
         free(head);
         return NULL;
     }

     return head;
 }

//DONE
/******************************************************************************
 * fileNames()
 *
 * Arguments: head - pointer to the LinkedList
 *            n_of_files - number of nodes in the list
 *
 * Returns: Pointer to an array of strings
 * Side-Effects: Allocates memory for the array.
 *
 * Description: Extracts filenames from the LinkedList and returns them as an
 *              array of strings.
 *
 *****************************************************************************/
char **fileNames(Lista *head, int n_of_files){

    char **files;
    int aux = 0;
    files = (char**)malloc(n_of_files*sizeof(char*));
    if (files == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for the array of strings\n");
        return NULL;
    }

    Lista *current= head;
    while(current!=NULL)
    {   files[aux] = (char*)malloc((strlen(current->word)+1)*sizeof(char));
        if (files[aux] == NULL)
        {
            fprintf(stderr, "Failed to allocate memory for the string\n");
            for (int i = 0; i <= aux; i++)
            {
                free(files[aux]);
            }
            free(files);
            return NULL;

        }

        strcpy(files[aux], current->word);
        aux++;
        current = current->next;
    }

    return files;

}

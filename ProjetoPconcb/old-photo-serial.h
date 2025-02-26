#ifndef OLD_PHOTO_SERIAL_H
#define OLD_PHOTO_SERIAL_H

#include <gd.h>

typedef struct thread_arg_t{
    char **files;
    int num_files;
    int threadid;
    gdImagePtr texture_img;
    char *directory;
    struct thread_arg_t *next;
    FILE *fptr;
} thread_arg_t;

gdImagePtr oldphoto(gdImagePtr in_img, gdImagePtr in_texture_img);
void free_string_array2(char **array, int size);
void freeThreadArgs(thread_arg_t *head);
void *process_photos(void *arg);
int old_photo_final(char **files,int nn_files,  int num_threads, char *dir,FILE *fptr);

#endif

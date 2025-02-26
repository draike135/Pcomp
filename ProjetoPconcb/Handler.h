#ifndef NOM3
#define NOM3

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include "Dynamic.h"

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
char *get_file_extension(char *filename);

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
void checkArgs(int argc, char *argv[]);

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
FILE *fileHandling(char *argv[]);

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
Lista *Directory(char *argv[]);

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
char **fileNames(Lista *head, int n_of_files);

#endif
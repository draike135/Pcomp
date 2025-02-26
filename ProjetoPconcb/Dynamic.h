#ifndef NOME
#define NOME

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Lista
{
    char *word;
    double size;
    struct Lista *next;
}Lista;

/******************************************************************************
 * printList()
 *
 * Arguments: head - pointer to head of the list
 *
 * Returns: none
 * Side-Effects: none
 *
 * Description: print LinkedList with every info in it
 *
 *****************************************************************************/
void printList(Lista *head);


/******************************************************************************
 * freeListas()
 *
 * Arguments: head - pointer to head of the list
 *
 * Returns: none
 * Side-Effects: Frees all dynamically allocated memory in the list
 *
 * Description: Traverses through the LinkedList and frees all nodes, including
 *              their dynamically allocated data (e.g., `word`).
 *
 *****************************************************************************/
void freeListas(Lista *head);

/******************************************************************************
 * free_string_array()
 *
 * Arguments: array - pointer to an array of strings
 *            size - number of elements in the array
 *
 * Returns: none
 * Side-Effects: Frees each string in the array and the array itself.
 *
 * Description: Safely deallocates memory for a dynamically allocated array of
 *              strings.
 *
 *****************************************************************************/
void free_string_array(char **array, int size); 

#endif
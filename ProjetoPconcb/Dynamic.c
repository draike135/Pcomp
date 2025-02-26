#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "Dynamic.h"

//Done
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
void printList(Lista *head) {
    Lista *current = head; //Start at the head
    int index = 0; //Index to know the node number

    printf("Printing the list:\n");
    while (current != NULL) {
        printf("Node %d:\n", index);
        printf("  Word: %s\n", current->word);
        printf("  Size: %.2f\n", current->size);
        printf("  Next: %p\n", (void *)current->next);
        current = current->next;
        index++;
    }

    printf("End of list.\n");
}
//Done
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
void freeListas(Lista *head){
    Lista *current = head; //Start at the head
    Lista *next;
    if (current->next!=NULL)
    {
        
        next = current->next;
        for (; current->next != NULL; next = current->next)
        {
            if (current->word!=NULL)
            {
                free(current->word);
            }
            free(current);
            current = next;

        }
        if (current->word!=NULL) //free the last node
        {
            free(current->word);
        }

        free(current);
    }else if (current!=NULL){
        if (current->word!=NULL)
        {
            free(current->word);
        }   
        free(current);
    }
        //free(current);

}
//Done
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
void free_string_array(char **array, int size) {
    if (array == NULL) return;

    for (int i = 0; i < size; i++){
        if (array[i] != NULL) {
            free(array[i]);
        }
    }

    free(array);
}
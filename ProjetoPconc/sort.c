#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Dynamic.h"
#include "sort.h"

//Done
/******************************************************************************
 * comparator()
 *
 * Arguments: flag - flag to determine if sorting is by size (1) or name (0)
 *            current - pointer to the current node being compared
 *            pivot - pointer to the pivot node for comparison
 *
 * Returns: 1 if the current node should precede the pivot based on the flag
 *          0 otherwise
 *
 * Side-Effects: none
 *
 * Description: Compares nodes based on file size or name. When comparing by
 *              name, the function performs a lexicographical comparison.
 *
 *****************************************************************************/
int comparator(int flag, Lista *current, Lista *pivot){
    int auxWarning;
    if (flag)
    {

        return (current->size < pivot->size); 

    }else{

        if (strlen(current->word)<strlen(pivot->word))
        {
            auxWarning = strlen(current->word);

            for (int i = 0; i < auxWarning; i++) //compare char by char and see which one is higher
            {
                if(current->word[i]<pivot->word[i])return 1;
            }

        }else{
            auxWarning = strlen(pivot->word);

            for (int i = 0; i < auxWarning; i++)
            {
                if(current->word[i]<pivot->word[i])return 1;
            }
        }



    }
    return 0;

}

//Done
/******************************************************************************
 * getTail()
 *
 * Arguments: cur - pointer to the head of the list
 *
 * Returns: Pointer to the last node in the list
 * Side-Effects: none
 *
 * Description: Traverses the list to find and return the last node.
 *
 *****************************************************************************/
Lista *getTail(Lista *cur){
    while (cur != NULL && cur->next != NULL)
        cur = cur->next;
    return cur;
}

//Done
/******************************************************************************
 * partition()
 *
 * Arguments: head - pointer to the head of the list
 *            left - pointer to the left sublist
 *            right - pointer to the right sublist
 *            flag - flag to determine sorting criteria (size or name)
 *
 * Returns: Pointer to the pivot node
 * Side-Effects: Reorganizes the list into two partitions around the pivot.
 *
 * Description: Splits the list into two parts based on the pivot element for
 *              QuickSort. Nodes smaller than the pivot go to the left, others
 *              to the right.
 *
 *****************************************************************************/
Lista* partition(Lista* head, Lista** left, Lista** right, int flag) {
    Lista* pivot = head;
    Lista* current = head->next;

    *left = NULL;
    *right = NULL;

    while (current) {
        Lista* nextNode = current->next;
        if (comparator(flag,current,pivot)) { //call comparator to check where to move the node
            current->next = *left;
            *left = current;
        } else {
            current->next = *right;
            *right = current;
        }
        current = nextNode;
    }

    pivot->next = NULL;
    return pivot;
}

//Done
/******************************************************************************
 * quickSort()
 *
 * Arguments: head - pointer to the head of the list
 *            flag - flag to determine sorting criteria (size or name)
 *
 * Returns: Pointer to the sorted list
 * Side-Effects: Reorganizes the list in place.
 *
 * Description: Recursively sorts the LinkedList using the QuickSort algorithm,
 *              combining the left, pivot, and right partitions into a sorted
 *              list.
 *
 *****************************************************************************/
Lista* quickSort(Lista* head, int flag) {
    if (!head || !head->next) {
        return head;
    }

    Lista *left = NULL, *right = NULL;
    Lista* pivot = partition(head, &left, &right, flag);

    left = quickSort(left,flag);
    right = quickSort(right,flag);


    Lista* sortedHead = left;
    if (left) {
        Lista* temp = left;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = pivot;
    } else {
        sortedHead = pivot;
    }

    pivot->next = right;
    return sortedHead;
}
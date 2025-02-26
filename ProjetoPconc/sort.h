#ifndef NOME2
#define NOME2

#include <stdio.h>
#include <stdlib.h>

#include "Handler.h"

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
int comparator(int flag, Lista *current, Lista *pivot);

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
Lista *getTail(Lista *cur);

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
Lista* partition(Lista* head, Lista** left, Lista** right, int flag);

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
Lista* quickSort(Lista* head, int flag);

#endif
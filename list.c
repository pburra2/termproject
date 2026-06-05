/* Priyanka Burra
 * Bhagyashri Tushir
 * Term Project
 * June 5, 2026 
 *
 * File: list.c
 *
 * Description:	This file contains the public functions and 
 *		        structures for a list abstract data type for generic
 *	        	pointer types.  The list supports deque operations, in
 *	        	which items can be easily added to or removed from the
 *	        	front or rear of the list. The operations include 
 *	        	creating, adding to, removing from, finding, getting, 
 *	         	switching out an item, counting, and destroying a list.
 *
 *	        	The list is a circular, doubly-linked list. Each node an
 *	        	array where the data is held, with each array doubling 
 *	        	capacity as each node gets added. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define MIN_SIZE 8

int maxsize = MIN_SIZE;


/* Struct definitions:
 * LIST: total count, dummy node, compare pointer function
 * NODE: local count, capacity, beginning index of array, data 
 *       array, next */
typedef struct list {
    int tcunt; 
    struct node *dummy;
    int (*compare)();
} LIST;

typedef struct node {
    int lcunt; 
    int cap;
    int ibeg;
    void **data;
    struct node *next;
    struct node *prev;
} NODE;


/* ---- PROJECT FUNCTIONS ---- */

/* 
 * - should have malloced list + head node
 * - node should have array??
 * - array has void *'s 
 * - assign head, compare, count = 0;
 * - head IS dummy node..?
 *
 * GOAL: return a pointer to a new list */

/* Takes: user compare function
 * Ouputs: list ptr
 *
 * Allocates memory for list, creates list. Allocates memory for 
 * dummy, creates it & assigns next and prev to itself to make a 
 * circular list. Assigns dummy capacity to 0, total count to 0.
 *
 * Big O(1) <-- no loops */
LIST *createList(int (*compare)()) {


    LIST *lp = malloc(sizeof(LIST));
    NODE *dummy = malloc(sizeof(NODE)); 

    lp->dummy = dummy;
    lp->compare = compare;
    lp->dummy->prev = dummy;
    lp->dummy->next = dummy;
    lp->dummy->cap = 0;
    lp->tcunt = 0;

    return lp;

}

/* Takes: list pointer, prev pointer, next pointer
 * Returns: new node pointer
 *
 * Allocates memory for and creates a newnode, doubling array size 
 * using the global max array size. Connects it to the linked list, 
 * assigns its beginning index to a default of 4, and local count 
 * to 0. This function is called in the add functions.
 *
 * Big O(1) <- no loops */
static NODE* mknode(LIST *lp, NODE* prev, NODE* next) {

    NODE* new = malloc(sizeof(NODE));

    new->data = malloc(sizeof(void*) * 2 * maxsize);
    new->cap = maxsize * 2;
    maxsize *= 2;
    
    new->lcunt = 0;
    new->ibeg = 4;
    new->prev = prev;
    new->next = next;
    prev->next = new;
    next->prev = new;

    return new;

}

/* Takes: list ptr
 * Ouputs: nothing
 * 
 * Destroys full list, freeing all memory previously allocated.
 * 
 * Big O(n) n is elements in list <- 1 while loop */
void destroyList(LIST *lp) {
    NODE *temp;
    while(lp->dummy->next != lp->dummy) {
        temp = lp->dummy->next;
        lp->dummy->next = temp->next;
        free(temp->data);
        free(temp);
    }

    free(lp->dummy);
    free(lp);
}

/* Takes: list ptr
 * Ouputs: # elements in list 
 * 
 * Gets number of items in list from total count;
 * 
 * Big O(1) <-- no traversing */
int numItems(LIST *lp) {
    assert (lp != NULL);
    return lp->tcunt;
}

/* Takes: list ptr, item ptr
 * Ouputs: nothing
 *
 * Adds element to beginning of list. Changes surrounding 
 * element ptrs as needed. Calls mknode if there no list nodes
 * present or if first node array is full.
 *
 * Note: I was told I don't need to half the size of a new first
 *       node.
 * 
 * Big O(1) <-- no loops */
void addFirst(LIST *lp, void *item) {

    assert (lp != NULL && item != NULL);

    NODE *curr;

    if (lp->tcunt == 0)     
        curr = mknode(lp, lp->dummy, lp->dummy);
    else
        curr = lp->dummy->next;

    if (curr->lcunt == curr->cap)
        curr = mknode(lp, lp->dummy, curr);
    if (curr->lcunt != 0) 
        curr->ibeg = (curr->ibeg - 1 + curr->cap) % curr->cap;

    curr->data[curr->ibeg] = item; 
    curr->lcunt++;
    lp->tcunt++;

}


/* Takes: list ptr, item ptr
 * Ouputs: nothing
 *
 * Adds element to end of list.
 * Changes surrounding element ptrs as needed.
 *
 * Adds element to end of list. Changes the surrounding 
 * element ptrs as needed. Calls mknode if there no list nodes
 * present or if last node array is full.
 *
 * Big O(1) <-- no loops */
void addLast(LIST *lp, void *item) {

    assert (lp != NULL && item != NULL);

    //so don't have to keep using pointers
    NODE *curr = lp->dummy->prev;
    int last;

    if (lp->tcunt == 0)    
        curr = mknode(lp, lp->dummy, lp->dummy); 
    else
        curr = lp->dummy->prev;

    if (curr->lcunt == curr->cap) 
        curr = mknode(lp, curr, lp->dummy);
    
    last = (curr->ibeg + curr->lcunt) % curr->cap; 
    curr->data[last] = item;
    curr->lcunt++;
    lp->tcunt++;

}

/* Takes: list ptr
 * Ouputs: item data, NULL if no items
 *
 * Removes first item of list. Changes surrounding element ptrs 
 * as needed.
 *
 * Big O(1) <-- no loops */
void *removeFirst(LIST *lp) {

    
    assert (lp != NULL);
    if (lp->tcunt == 0)   
        return NULL;

    NODE *temp, *curr = lp->dummy->next;
    void *value;

    if (curr->lcunt == 0) { 
        temp = curr;
        curr = curr->next;
        curr->prev = lp->dummy;
        lp->dummy->next = curr;
        free(temp->data);
        free(temp);
    }

    value = curr->data[curr->ibeg];
    curr->data[curr->ibeg] = NULL;
    curr->ibeg = (curr->ibeg + 1) % curr->cap;
    curr->lcunt--;
    lp->tcunt--;

    return value;

}

/* Takes: list ptr
 * Ouputs: item data, NULL if no items
 *
 * Removes last item of list. Changes surrounding element ptrs as
 * needed.
 *
 * Big O(1) <-- no loops */
void *removeLast(LIST *lp) {

    assert (lp != NULL);

    if (lp->tcunt == 0) //no nodes    
        return NULL;
    
    NODE *temp, *curr = lp->dummy->prev;
    int last;
    void *value;

    if (curr->lcunt == 0) { 
        temp = curr;
        curr = curr->prev;            
        curr->next = lp->dummy;
        lp->dummy->prev = curr;
        free(temp->data);
        free(temp);
    }
    last = (curr->ibeg + curr->lcunt - 1) % curr->cap;

    value = curr->data[last];
    curr->data[last] = NULL;
    curr->lcunt--;
    lp->tcunt--;

    return value;

}

/* Takes: list ptr, item ptr
 * Ouputs: item data, NULL if invalid index/no items in list
 *
 * Gets data of element of list at given index.
 * 
 * Big O(log n) n is # of nodes in list 
 * <- traversing through nodes that have double the items as 
 *    the previous node */
void *getItem(LIST *lp, int index) {

    
    if (lp->tcunt == 0) { //no nodes
        printf("No items in list\n");
        return NULL;
    }
    else if (index >= lp->tcunt || index < 0) {
        printf("Index value is invalid\n");
        return NULL;
    }
    else {
        NODE *ncurr = lp->dummy->next;
        while (index - ncurr->cap >= 0 && ncurr->cap != 0) {
            index -= ncurr->cap;
            ncurr = ncurr->next;
        }

        return ncurr->data[(ncurr->ibeg + index) % ncurr->cap];
    }

}


/* Takes: list ptr, item ptr
 * Ouputs: nothing
 *
 * Switches out data of element of list at given index with 
 * given value
 * 
 * Big O(log n) n is # of nodes in list 
 * <- traversing through nodes that have double the items as 
 *    the previous node */
void setItem(LIST *lp, int index, void *item) {

    if (lp->tcunt == 0) //no nodes
        printf("No items in list\n");
    else if (index >= lp->tcunt || index < 0) 
        printf("Index value is invalid\n");
    else {
        NODE *ncurr = lp->dummy->next;
        while (index - ncurr->cap >= 0 && ncurr->cap != 0) {
            index -= ncurr->cap;
            ncurr = ncurr->next;
        }

        ncurr->data[(ncurr->ibeg + index) % ncurr->cap] = item;

    }

}

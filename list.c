/* Priyanka Burra
 * Bhagyashri Tushir
 * Term Project
 * June 5, 2026 
 *
 * File: list.c
 *
 * Description:
 *
 *
 * when addingfirst/removefirst, do you have to update what the "first"
 * el array loc is?
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define ARRAY_SIZE 8


//define structs
typedef struct list {
    int tcunt; //total count
    struct node *dummy;
    int (*compare)(); //do we need?
} LIST;

typedef struct node {
    int lcunt; //local count
    int cap; //capacity of array
    int ibeg; //beginning index
    void **data; //node array of data
    struct node *next;
    struct node *prev;
} NODE;


/* Brainstorm:
 * - should have malloced list + head node
 * - node should have array??
 * - array has void *'s 
 * - assign head, compare, count = 0;
 * - head IS dummy node..?
 *
 * GOAL: return a pointer to a new list */
LIST *createList(int (*compare)()) {

    LIST *lp = malloc(sizeof(LIST));
    NODE *dummy = malloc(sizeof(NODE)); 
    
    lp->dummy = dummy;
    lp->dummy->prev = dummy;
    lp->dummy->next = dummy;
    lp->compare = compare;
    lp->tcunt = 0;

    return lp;

}

/* private func making a new node with goal of adding 
 * malloc
 * if list has no nodes, make localcnt, totalcnt, assign
 * respective prevs/nexts
 * if list has nodes, malloc, assign prev/nexts
 * */
static NODE* mknode(LIST *lp, NODE* prev) {
    
    NODE* new = malloc(sizeof(NODE));

    if (prev->cap == NULL) {
        new->data = malloc(sizeof(void*) * ARRAY_SIZE);
        new->cap = ARRAY_SIZE;
    }
    else {
        new->data = malloc(sizeof(void*) * prev->cap * 2);
        new->cap = prev->cap * 2;
    }
    
    new->lcunt = 0;
    new->ibeg = 4;
    new->prev = prev;
    new->next = lp->dummy;
    prev->next = new;
    prev->prev = new;
    lp->tcunt++;

    return new;

}

/* Brainstorm:
 * - at end, free all arrays + full list
 *
 *
 * GOAL: deallocate memory associated with the 
 *       list pointed to by lp */
void destroyList(LIST *lp) {

   free(lp->dummy);
   free(lp);

}

//return the number of items in the list pointed to by lp
int numItems(LIST *lp) {
    assert (lp != NULL);
    return lp->tcunt;
}

//add item as the first item in the list pointed to by lp
void addFirst(LIST *lp, void *item) {

    assert (lp != NULL && item != NULL);

    //so don't have to keep using pointers
    NODE *curr;

    if (lp->tcunt == 0)  //no nodes    
        curr = mknode(lp, lp->dummy);
    else {
        curr = lp->dummy->next;
        if (curr->lcunt != 0)
            curr->ibeg = (curr->ibeg - 1) % curr->cap; 
    }
    
    curr->data[curr->ibeg] = item; 
    curr->lcunt++;
    lp->tcunt++;

}

//add item as the last item in the list pointed to by lp
void addLast(LIST *lp, void *item) {

    assert (lp != NULL && item != NULL);

    //so don't have to keep using pointers
    NODE *curr;
    
    if (lp->tcunt == 0) { //no nodes    
        curr = mknode(lp, lp->dummy);
    }
    else 
        curr = lp->dummy->prev;
               
    curr->data[curr->ibeg + curr->lcunt % curr->cap] = item;
    curr->lcunt++;
    lp->tcunt++;

}

//remove and return the first item in the list pointed to by lp
//the list must not be empty
void *removeFirst(LIST *lp) {

    assert (lp != NULL);

    //so don't have to keep using pointers
    NODE *curr;
    int loc;
    void *value;
    
    if (lp->tcunt == 0) { //no nodes    
        return NULL;
    }
    else {
        curr = lp->dummy->next;
        if (curr->lcunt == 0) { //if array is empty
            void *temp = curr;
            curr = curr->next;
            curr->prev = lp->dummy;
            free(temp);
        }
        loc = slotf;
    }
        
    value = curr->data[loc];
    curr->data[loc] = NULL;
    curr->lcunt--;
    lp->tcunt--;

    return value;

}

//remove and return the last item in the list pointed to by lp ; the list must not be empty
void *removeLast(LIST *lp) {

    assert (lp != NULL);

    //so don't have to keep using pointers
    NODE *curr;
    int loc;
    void *value;
    
    if (lp->tcunt == 0) { //no nodes    
        return NULL;
    }
    else {
        curr = lp->dummy->prev;
        if (curr->lcunt == 0) { //if array in last node empty, but
            void *temp = curr;
            curr = curr->prev;
            curr->next = lp->dummy;
            free(temp);
        }

        loc = (slotf + curr->lcunt) % curr->cap;
    }
 
    value = curr->data[loc];
    curr->data[loc] = NULL;
    curr->lcunt--;
    lp->tcunt--;

    return value;

}

//return the item at position index in the list pointed to by lp
//the index must be within range
void *getItem(LIST *lp, int index) {

   if (lp->tcunt == 0) {//no nodes
       printf("No items in list");
       return NULL;
   }
   else if (index >= lp->tcunt || index < 0) {
       printf("Index value is invalid");
       return NULL;
   }
   else {
       NODE *ncurr = lp->dummy->next;
       int icurr = 0; 
       while (index > (icurr + ncurr->lcunt)) {
            icurr += ncurr->lcunt;
            ncurr = ncurr->next;
       }

       return ncurr->data[slotf + (index - icurr)];
       
   }

}

//change the item at position index in the list pointed to by lp
//the index must be within range
void setItem(LIST *lp, int index, void *item) {

   if (lp->tcunt == 0) //no nodes
       printf("No items in list");
   else if (index >= lp->tcunt || index < 0) 
       printf("Index value is invalid");
   else {
       NODE *ncurr = lp->dummy->next;
       int icurr = 0; 
       while (index > (icurr + ncurr->lcunt)) {
            icurr += ncurr->lcunt;
            ncurr = ncurr->next;
       }

       ncurr->data[slotf + (index - icurr)] = item;
       
   }

}

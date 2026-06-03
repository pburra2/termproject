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
 * el array loc is? me: i think so
 *
 * why do my sizes double when there is no search for a certain value?
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define ARRAY_SIZE 8


//define structs
typedef struct list {
    int tcunt; //total count
    struct node *dummy;
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

    printf("before createlist\n");
    LIST *lp = malloc(sizeof(LIST));
    NODE *dummy = malloc(sizeof(NODE)); 

    lp->dummy = dummy;
    lp->dummy->prev = dummy;
    lp->dummy->next = dummy;
    lp->dummy->cap = 0;
    lp->tcunt = 0;

    printf("list created\n");
    return lp;


}

/* private func making a new node with goal of adding 
 * malloc
 * if list has no nodes, make localcnt, totalcnt, assign
 * respective prevs/nexts
 * if list has nodes, malloc, assign prev/nexts
 * */
static NODE* mknode(LIST *lp, NODE* prev, NODE* next) {

    NODE* new = malloc(sizeof(NODE));

    if (prev->cap == 0) {
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
    new->next = next;
    prev->next = new;
    next->prev = new;

    return new;

}

/* Brainstorm:
 * - at end, free all arrays + full list
 *
 *
 * GOAL: deallocate memory associated with the 
 *       list pointed to by lp */
void destroyList(LIST *lp) {
    NODE *temp;
    //free nodes
    while(lp->dummy->next != lp->dummy) {
        temp = lp->dummy->next;
        lp->dummy->next = temp->next;
        free(temp->data);
        free(temp);
    }
    
    //free list
    free(lp->dummy);
    free(lp);
    printf("list destroyed\n");
}

//return the number of items in the list pointed to by lp
int numItems(LIST *lp) {
    assert (lp != NULL);
    return lp->tcunt;
}

//add item as the first item in the list pointed to by lp
//if first node's array full, allocate new first node
//  <- should I half the size??
//  ans: TA said that'll never happen
void addFirst(LIST *lp, void *item) {

    assert (lp != NULL && item != NULL);

    //so don't have to keep using pointers
    NODE *curr;

    if (lp->tcunt == 0)  //no nodes    
        curr = mknode(lp, lp->dummy, lp->dummy);
    else 
        curr = lp->dummy->next;
    
    if (curr->lcunt == curr->cap) //array full
        curr = mknode(lp, lp->dummy, curr);
    if (curr->lcunt != 0) //not empty array
        curr->ibeg = (curr->ibeg + curr->cap - 1) % curr->cap; //add b so never negative
    //otherwise ibeg stays ibeg

    curr->data[curr->ibeg] = item; 
    curr->lcunt++;
    lp->tcunt++;

}

//add item as the last item in the list pointed to by lp
void addLast(LIST *lp, void *item) {

    printf("beg add, count = %d\n", lp->dummy->prev->lcunt);
    assert (lp != NULL && item != NULL);

    //so don't have to keep using pointers
    NODE *curr;
    int last;

    if (lp->tcunt == 0)  //no nodes    
        curr = mknode(lp, lp->dummy, lp->dummy); 
    else
        curr = lp->dummy->prev;
    
    if (curr->lcunt == curr->cap) //array full
        curr = mknode(lp, curr, lp->dummy);
    
    last = (curr->ibeg + curr->lcunt - 1) % curr->cap; 
    curr->data[last] = item;
    curr->lcunt++;
    lp->tcunt++;

    printf("end add");
}

//remove and return the first item in the list pointed to by lp
//the list must not be empty
void *removeFirst(LIST *lp) {
    
    assert (lp != NULL);
    if (lp->tcunt == 0) //no nodes    
        return NULL;
    
    //so don't have to keep using pointers
    NODE *temp, *curr = lp->dummy->next;
    void *value;

    if (curr->lcunt == 0) { //if array is empty
        temp = curr;
        curr = curr->next;
        curr->prev = lp->dummy;
        lp->dummy->next = curr;
        free(temp->data);
        free(temp);
    }
    //if not empty, ibeg stays the same (first)

    value = curr->data[curr->ibeg];
    curr->data[curr->ibeg] = NULL;
    curr->ibeg = (curr->ibeg + curr->cap + 1) % curr->cap;
    curr->lcunt--;
    lp->tcunt--;

    return value;

}

//remove and return the last item in the list pointed to by lp ; the list must not be empty
void *removeLast(LIST *lp) {

    printf("beg remove, count = %d\n", lp->dummy->prev->lcunt);
    assert (lp != NULL);
    if (lp->tcunt == 0) //no nodes    
        return NULL;
    
    //so don't have to keep using pointers
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
        while (index >= (icurr + ncurr->lcunt - 1)) {
            icurr += ncurr->lcunt;
            ncurr = ncurr->next;
        }

        return ncurr->data[ncurr->ibeg + (index - icurr)];
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
        while (index >= (icurr + ncurr->lcunt - 1)) {
            icurr += ncurr->lcunt;
            ncurr = ncurr->next;
        }

        ncurr->data[ncurr->ibeg + (index - icurr) % ncurr->cap] = item;

    }

}

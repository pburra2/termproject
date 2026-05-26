/* Priyanka Burra
 * Bhagyashri Tushir
 * Term Project
 * June 5, 2026 
 *
 * File: list.c
 *
 * Description:
 *
 */


//define structs
typedef struct list {
    int count;
    struct node *head;
    int (*compare)(); //do we need?
} LIST;

typedef struct node {
    void *data;
    void **elts; //array for each node
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
LIST *createList(void) {



}

/* Brainstorm:
 * - at end, free all arrays + full list
 *
 *
 * GOAL: deallocate memory associated with the 
 *       list pointed to by lp */
void destroyList(LIST *lp) {

    

}

//return the number of items in the list pointed to by lp
int numItems(LIST *lp) {



}

//add item as the first item in the list pointed to by lp
void addFirst(LIST *lp, void *item) {



}

//add item as the last item in the list pointed to by lp
void addLast(LIST *lp, void *item) {



}

//remove and return the first item in the list pointed to by lp ; the list must not be empty
void *removeFirst(LIST *lp) {



}

//remove and return the last item in the list pointed to by lp ; the list must not be empty
void *removeLast(LIST *lp) {



}

//return the item at position index in the list pointed to by lp ; the index must be within range
void *getItem(LIST *lp, int index) {



}

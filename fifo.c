#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

extern int memsize;

extern int debug;

extern struct frame *coremap;

//using circular-queue for fifo
int *queue;
int front;
int rear;

/* Page to evict is chosen using the fifo algorithm
 * Returns the slot in the coremap that held the page that
 * was evicted.
 */

int fifo_evict(struct page *p) {
	//take first one and put it to last after eviction
	int i = del();
	insert(i);
	// find the victime page in the pagetable and mark
	// it as not in memory
	struct page *victim = find_page(coremap[i].vaddr);
	victim->pframe = -1;
	
	return i;
}

/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void fifo_init() {
	queue = (int *)malloc(sizeof(int)*memsize);
	int i = 0;
	while(i<memsize){
		queue[i] = i;
		i++;	
	}

}

// insert to the end of queue
void insert(int t)
{
	if(rear == memsize){
		rear = 0;
	}
     queue[rear]=t;
     rear++;
    
}

//delete from the front of queue
int del()
{
	if(front == memsize){
		front = 0;
	}
	int q = queue[front];
    queue[front]=0;
    front++;
    return q;
}

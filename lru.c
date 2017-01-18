#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

extern int memsize;

extern int debug;

extern struct frame *coremap;

/* Page to evict is chosen using the accurate LRU algorithm 
 * Returns the slot in the coremap that held the page that
 * was evicted.
 */

//array size of memsize for sorting frames
int *array;

int lru_evict(struct page *p) {
	//first element in array is always least used 
	//lru_sort handles that for us
	int i = array[0];
	struct page *victim = find_page(coremap[i].vaddr);
	victim->pframe = -1;
	//whenever new page is inserted we need re-sort
	lru_sort(i);
	return i;
}


/* Initialize any data structures needed for this 
 * replacement algorithm 
 */
void lru_init() {
	//initialize array and allocate memory
	array = (int *)malloc(sizeof(int)*memsize);
	int i = 0;
	//array keeps frames and sort it whenever needed
	while(i<memsize){
		array[i] = i;
		i++;
	}	
}

/*this function sort according to their past usage
this function is called whenever hits happens and new 
page is inserted*/
void lru_sort(int frame){
	//find the frame number in array
	int i = search(frame);
	//while i+1 frame is in use swap their places
	while(i<memsize && coremap[i+1].in_use){
		array[i] = array[i+1];
		i++;
	}
	//if coremap is full put frame num in last index
	if(i==memsize){
		array[i-1] = frame;}
	//else put frame num at index i	
	else{
		array[i] = frame;
	}
	
}

// search frame num in array and return index
int search(int frame){
	int i = 0;
	while(i<memsize){
		if(array[i] == frame){
			break;
		}
		i++;
	}
	return i;
}

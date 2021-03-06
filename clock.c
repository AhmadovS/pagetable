#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"


extern int memsize;

extern int debug;

//rec is using as clockhand
extern int rec;

extern struct frame *coremap;

/* Page to evict is chosen using the accurate clock algorithm 
 * Returns the slot in the coremap that held the page that
 * was evicted.
 */

int clock_evict(struct page *p) {
	int i;
	int ev = 0;
	//if clockhand was in the last frame before eviction
	//move it to first one
	if(rec == memsize){
		rec = 0;
	}
	i = rec;
	while(i < memsize){
		//start from clockhand and if .ref is 0 return that
		if(!coremap[i].ref){
			struct page *victim = find_page(coremap[i].vaddr);
			victim->pframe = -1;
			ev = i;
			break;}
		//else flip ref attribute to 0
		if(coremap[i].ref){
			coremap[i].ref = 0;
		}
		
		i++;
		//if all frames from clockhand to last one has ref attr 1
		//move to first frame
		if(rec == memsize-1){
			rec = 0;
			i = rec;
		}
	}

	return ev;
}

/* Initialize any data structures needed for this replacement
 * algorithm 
 */
void clock_init() {
}

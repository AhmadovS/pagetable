#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include "pagetable.h"

#define MAXLINE 256
extern int memsize;

extern char *tracefile;

extern int debug;

extern struct frame *coremap;

//head and tail of linked list
opt_list *traces = NULL;
opt_list *tail = NULL;


/* Page to evict is chosen using the accurate optimal algorithm 
 * Returns the slot in the coremap that held the page that
 * was evicted.
 */

int opt_evict(struct page *p) {
	int slot = 0;
	int i = 0;
	int largest = 0;
	int index = 0;
	while(slot<memsize){
		//first index of frame number
		i = find_index(coremap[slot].vaddr);
		if(i==memsize){
			break;
		}
		//if find the frame that will be used 
		//latest update the index
		if(i>largest){
			largest = i;
			index = slot;
		}
		slot++;

	}
	struct page *victim = find_page(coremap[index].vaddr);
	victim->pframe = -1;
	//delete address from linked list
	delete_opt();
	//display();
	return index;
}

/* Initializes any data structures needed for this
 * replacement algorithm. Open file and read address
 * insert it to linked list
 */
void opt_init(addr) {
	char buf[256];
	FILE *infp;
	addr_t vaddr;
	char type;
	int length;
	if((infp = fopen(tracefile, "r"))==NULL){
		perror("No file found");
		exit(1);
	}
	//no need to insert first memsize elements to list
	while((vaddr & ~0xfff)!= addr && fgets(buf,MAXLINE,infp)!= NULL) {
		sscanf(buf, " %c %lx,%u", &type, &vaddr, &length);

		}
	insert_opt(vaddr);


	while(fgets(buf,MAXLINE,infp)) {
		if(buf[0] != '=') {
			sscanf(buf, " %c %lx,%u", &type, &vaddr, &length);
			insert_opt(vaddr);
	}}

}

//insert address to the end of linked list
void insert_opt(addr_t vaddr){

	struct opt *temp = malloc(sizeof(opt_list));
	if(!traces){
		tail = malloc(sizeof(opt_list));
		traces = malloc(sizeof(opt_list));
		tail->vaddr = vaddr;
		tail->next = NULL;
		traces = tail;
	}

	else{
		temp->vaddr = vaddr;
		temp->next = NULL;
		tail->next = temp;
		tail = tail->next;
	}
}

//delete address after using it
void delete_opt(){
	traces = traces->next;
}


//find the first index of address from linked list
int find_index(addr_t vaddr){
	opt_list *tmp = malloc(sizeof(opt_list));
	tmp  = traces;
	int i = -1;
	while(tmp){
		i++;
		if((tmp->vaddr& ~0xfff)==vaddr){
			break;
		}

		tmp = tmp->next;
	}
	return i;
}

//display the linked list
void display(){
	opt_list *tmp = malloc(sizeof(opt_list));
	tmp = traces;
	while(tmp != NULL){
		printf("traces %lx\n",tmp->vaddr );
		tmp = tmp->next;
	}
}

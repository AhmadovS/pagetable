#include <stdio.h>
#include <stdlib.h>
#include "avl.h"

/* The avl tree is a simple way to get a fast look up table, and is
 * used to implement the page table.
 */
extern struct avl_table *avl_tree;
extern struct libavl_allocator avl_allocator_default;

typedef unsigned long addr_t;
typedef struct opt opt_list;

//linked list to keep track of addresses for opt algorithm
struct opt
{
	addr_t vaddr;
	opt_list *next;
};

struct page {
	addr_t vaddr; // key
	char type;    // Instruction or data
	int pframe;   // Page frame number. -1 if not in physical memory
};


int page_cmp(const void *a, const void *b, void *p); 
void init_pagetable();

struct page *pagetable_insert(addr_t vaddr, char type);
struct page *find_page(addr_t vaddr);
void print_pagetable(void);

struct frame {
	char in_use;   //
	char type;     //Instruction (I) or Data (D)
	addr_t vaddr;
	int  ref;	//bit used for clock algortihm
};

void rand_init();
void lru_init();
void clock_init();
void fifo_init();
void opt_init();

int rand_evict(struct page *p);
int lru_evict(struct page *p);
int clock_evict(struct page *p);
int fifo_evict(struct page *p);
int opt_evict(struct page *p);

//necessary helper functions for algorithms
void insert_opt(addr_t vaddr);
void delete_opt();
int find_index(addr_t vaddr);
void insert(int t);
int del();
int search(int frame);
void lru_sort(int frame);
void display();

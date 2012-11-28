#ifndef BP_MEMORY
#define BP_MEMORY

#define bp_alloc1(size) (malloc (size))
#define bp_alloc2(num_items, size) (calloc (num_items, size))
#define bp_alloc3(arr, size) (realloc (arr, size))
#define NEW(type) ((type *)(bp_alloc2 (1, sizeof (type))))
#define NEW_0(type, size) ((type *)(bp_alloc2 ((size), sizeof (type))))
#define NEW_1(type, pool) ((type *)bp_mempool_alloc (pool, sizeof (type)))
#define NEW_2(type, arr, size)((arr) = (type *)(bp_alloc3 (arr, size * sizeof (type))))

#include <stdlib.h>

/*
  It is important that the contents of this struct are aligned well
     x86    : 16 bytes
     x86_64 : 32 bytes

  We don't need any padding on these architectures when using gcc cos gcc
     aligns things on a sizeof (struct _bp_mempool_t *) boundaries.
*/
typedef struct _bp_mempool_node_t {
	struct _bp_mempool_node_t *next;
	char *start;
	char *pos;
	unsigned short rest;
	unsigned short size;
} bp_mempool_node_t;

typedef struct {
	struct _bp_mempool_node_t *curr;
} bp_mempool_t;

typedef struct {
	char *array;
	unsigned char item_size;
	unsigned int num_items : 24;
	unsigned int max_num_items;
} bp_arraylist_t;
	
bp_mempool_t *
bp_mempool_new      ();

void 
bp_mempool_destroy  (bp_mempool_t *pool);

char *
bp_mempool_alloc    (bp_mempool_t *pool, unsigned int size);

bp_arraylist_t *
bp_arraylist_new (unsigned char member_size, unsigned int num_items);

void
bp_arraylist_grow (bp_arraylist_t *alist, unsigned int size_delta);

char *
bp_arraylist_alloc (bp_arraylist_t *alist);

void bp_arraylist_alloc1 (bp_arraylist_t *alist, void *val);

void bp_arraylist_set (const bp_arraylist_t *alist, int pos, void *val);

void
bp_arraylist_destroy (bp_arraylist_t *alist);

#define bp_arraylist_get(list, i) (list->array + list->item_size * i)
#define bp_arraylist_get_size(list) (list->num_items)


#endif

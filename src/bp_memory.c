#include <bp_utils.h>
#include <bp_scene.h>
#include <string.h>

#define MEMPOOL_PAGESIZE 8192
#define MEM_ALIGN 8

bp_mempool_t *
bp_mempool_new ()
{
	bp_mempool_t *pool = NEW (bp_mempool_t);
	bp_mempool_node_t *node = NEW_0 (bp_mempool_node_t , MEMPOOL_PAGESIZE);

	node->next  = NULL;
	node->start = node->pos  = (char *)node + sizeof (bp_mempool_node_t);
	node->size  = node->rest = MEMPOOL_PAGESIZE - sizeof (bp_mempool_node_t);

	pool->curr = node;

	return pool;
}

void
bp_mempool_destroy (bp_mempool_t *pool)
{
	bp_mempool_node_t *node, *next;

	if (!pool)
		return;

	node = pool->curr;
	while (node) {
		next = node->next;
		free (node);
		node = next;
	}

	free (pool);
}


char *
bp_mempool_alloc (bp_mempool_t *pool, unsigned int size)
{
	char *rval;
	bp_mempool_node_t *node = pool->curr;

	if (node->rest < size) {
		bp_mempool_node_t *nn = NEW_0 (bp_mempool_node_t, MEMPOOL_PAGESIZE);

		nn->next   = node;
		nn->start  = nn->pos  = (char *)nn + sizeof (bp_mempool_node_t);
		nn->size   = nn->rest = MEMPOOL_PAGESIZE - sizeof (bp_mempool_node_t);
		pool->curr = node = nn;
	}
	
	rval        = node->pos;
	node->rest -= size;
	node->pos  += size;

	return rval;
}


bp_arraylist_t *
bp_arraylist_new (unsigned char item_size, unsigned int num_items)
{
	bp_arraylist_t *list = NEW (bp_arraylist_t);

	list->item_size = item_size;
	list->max_num_items = num_items;
	list->array = NEW_0 (char, list->max_num_items * item_size);
	list->num_items = 0;
	return list;
}


/**
 * Returns position in the array that we are going to insert into next
 */
char *
bp_arraylist_alloc (bp_arraylist_t *alist)
{
	char *pos;
	
	if (alist->num_items == alist->max_num_items) {
		/* FIXME is this faster than max_items = max_items * 3 / 2 */
		alist->max_num_items += alist->max_num_items / 2;
 		alist->array = bp_alloc3 (alist->array, alist->max_num_items * alist->item_size);
	}

	pos = alist->array + (alist->num_items * alist->item_size);
	alist->num_items ++;
	return pos;
}

void
bp_arraylist_alloc1 (bp_arraylist_t *alist, void *val)
{
	char *dest = bp_arraylist_alloc (alist);
	memcpy (dest, val, alist->item_size);
}

void 
bp_arraylist_set (const bp_arraylist_t *alist, int pos, void *val)
{
	memcpy (bp_arraylist_get (alist, pos), val, alist->item_size);
}

void
bp_arraylist_grow (bp_arraylist_t *alist, unsigned int size_delta)
{
	alist->max_num_items += size_delta;
	alist->array = bp_alloc3 (alist->array, alist->max_num_items * alist->item_size);
}
	
	
void
bp_arraylist_destroy (bp_arraylist_t *alist)
{
	free (alist->array);
}

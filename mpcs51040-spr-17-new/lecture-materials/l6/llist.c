#include "llist.h"
#include <assert.h>

void llist_create(llist_t * l)
{
   assert(l);
   l->data = NULL;
   l->next = NULL;
}

listnode_t * node_create()
{
   listnode_t * n = (listnode_t *) malloc (sizeof(listnode_t));
   return n;
}

void llist_insert_begin(llist_t * l, void * data)
{
   llnode_t * n = node_create();
   n->data = data;
   n->next = l->first;
   l->first = n;
}

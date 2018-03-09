#include "llist.h"

int main ()
{
   llist_t l;
   llist_create(&l);

   llist_insert_begin(&l, (void *) 8); 
   llist_insert_begin(&l, (void *) 2); 
   llist_destroy(&l)

}

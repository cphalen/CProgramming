#include "trie.h"

struct trie_data_t {
  trie_free_t freeData;
  trie_pos_t root;
}

struct trie_node_t {
  const char * key;
  unsigned int numberChildren;
  trie_pos_t children[];
  void * data;
}

void trie_destroy (trie_t trie, trie_free_t freefunc) {
    trie_destroy_recurse(trie->root, freefunc);
}

static void trie_destroy_recurse(trie_pos_t node, freefunc) {
  if(!node)
    return;

  // Recurses to run through each and every single node
  // It makes no difference how the tree is walked -- everything
  // will be free regardless
  freefunc(trie->data);
  for(int i = 0; i < trie->numberChildren; i++)
    trie_destroy(((trie->children)[i])->data);
  free(trie);
}

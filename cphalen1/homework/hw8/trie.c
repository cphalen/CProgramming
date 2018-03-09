#include "trie.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

struct trie_data_t {
  trie_pos_t * root;
  char ** keys;
  unsigned int numberRootPaths;
  unsigned int size;
};

struct trie_node_t {
  char key;
  unsigned int numberChildren;
  trie_pos_t * children;
  void * data;
};

trie_t trie_new() {
  // Initialize all values to 0 or NULL
  // We cannot malloc a default value for root or keys
  // as they point to a pointer of type trie_pos_t and char
  // respectively, thus their default values must point to
  // 0 trie_pos_t values and 0 char values (which a NULL
  // pointer acomplishes)
  trie_t returnTrie = (trie_t) malloc(sizeof(struct trie_data_t));
  returnTrie->root = NULL;
  returnTrie->keys = NULL;
  returnTrie->numberRootPaths = 0;
  returnTrie->size = 0;

  return returnTrie;
}

static void trie_destroy_recurse(trie_pos_t pos, trie_free_t freefunc) {
  if(!pos)
    return;

  // Recurses to run through each and every single node
  // It makes no difference how the tree is walked -- everything
  // will be free regardless
  if(freefunc != NULL) {
    freefunc(pos->data);
  }
  for(int i = 0; i < pos->numberChildren; i++)
    if(i == 0) {
      trie_destroy_recurse((pos->children)[i], freefunc);
    }
  free(pos);
}

void trie_destroy(trie_t trie, trie_free_t freefunc) {
  if(!trie)
    return;

  for(int i = 0; i < trie->numberRootPaths; i++)
    trie_destroy_recurse((trie->root)[i], freefunc);
}

void * trie_get_value(const trie_t trie, trie_pos_t pos) {
  return pos->data;
}

void trie_set_value (trie_t trie, trie_pos_t pos, void * value) {
  pos->data = value;
}

unsigned int trie_size(const trie_t trie) {
  return trie->size;
}

trie_pos_t trie_find_recurse(trie_pos_t pos, const char * key, int iterations) {
  // Check to see if the current key characters line up, and if this character
  // comparison concludes the string
  if(pos->key == *(key + iterations - 1) && strlen(key) == iterations)
    return pos;

  if(pos == TRIE_INVALID_POS)
    return TRIE_INVALID_POS;

  trie_pos_t returnTrie = TRIE_INVALID_POS;
  for(int i = 0; i < pos->numberChildren; i++) {
    // Looks to compare all children where the keys have identical values
    if((pos->children)[i]->key == *(key + iterations)) {
      returnTrie = trie_find_recurse((pos->children)[i], key, iterations + 1);
      if(returnTrie != TRIE_INVALID_POS)
        return returnTrie;
    }
  }

  return TRIE_INVALID_POS;
}

trie_pos_t trie_find(const trie_t trie, const char * key) {
  if(!trie || !key)
    return TRIE_INVALID_POS;

  trie_pos_t returnTrie;
  // Continues outwards from the root -- in this model
  // the first pass is iteration 1, so a value of 1 is passed
  // to parameter iterations
  for(int i = 0; i < trie->numberRootPaths; i++) {
    returnTrie = trie_find_recurse(*(trie->root + i), key, 1);
    if(returnTrie != TRIE_INVALID_POS) {

      return returnTrie;
    }
  }
  return TRIE_INVALID_POS;
}

bool trie_walk_recurse(trie_t trie, trie_pos_t pos, trie_walk_t walkfunc, void * priv, char * key) {
  if(!pos)
    return false;
  // Base case being that the current position is NULL

  // Incremement the key by one -- this is actually more effective
  // than the iterations method
  *key = pos->key;
  if(walkfunc(trie, pos, ++key, priv) == false)
    return false;

  for(int i = 0; i < pos->numberChildren; i++) {
    // Walk for all existing children
    if(trie_walk_recurse(trie, (pos->children)[i], walkfunc, priv, key) == false)
      return false;
  }

  return true;
}

bool trie_walk(trie_t trie, trie_walk_t walkfunc, void * priv) {
  if(!walkfunc || trie || priv)
    return false;
  char * key = malloc(sizeof(char) * trie->size);
  // Recurses towards from the root for every key
  for(int i = 0; i < trie->numberRootPaths; i++) {
    if(trie_walk_recurse(trie, (trie->root)[i], walkfunc, priv, key) == false)
      return false;
  }
  return true;
}

bool trie_insert_recurse(trie_t trie, const char * str, void * newval, trie_pos_t * newpos) {
  if(!trie || !str)
    return false;

  if(trie_find(trie, str)) {
    return false;
  }

  // Check to see how far back the closet subkey exists
  trie_pos_t parentRoot;
  int iterations = 0;
  char newKey[strlen(str)];
  memcpy(newKey, str, strlen(str));
  for(int i = 0; i < strlen(str); i++) {
    newKey[strlen(str) - i] = '\0';
    if((parentRoot = trie_find(trie, newKey)) != TRIE_INVALID_POS) {
      iterations = i;
      break;
    }
  }

  // Create the new position we will be adding
  trie_pos_t insertpos = (trie_pos_t) malloc(sizeof(struct trie_node_t));
  insertpos->key = str[strlen(str) - 1];
  if(!newval)
    insertpos->data = NULL;
  else
    insertpos->data = newval;
  insertpos->numberChildren = 0;

  // Insert newpos below parent
  if(parentRoot != TRIE_INVALID_POS) {
    if(iterations == 1) {
      // If the value is one up the tree, simply add it as a child
      // of that value
      trie_pos_t * temp = (trie_pos_t *) malloc(sizeof(trie_pos_t) * parentRoot->numberChildren);
      for(int i = 0; i < parentRoot->numberChildren; i++) {
        *(temp + i) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
        memcpy(*(temp + i), *(parentRoot->children + i), sizeof(struct trie_node_t));
        free(*(parentRoot->children + i));
      }

      parentRoot->children = (trie_pos_t *) malloc(sizeof(trie_pos_t) * (parentRoot->numberChildren + 1));
      for(int i = 0; i < parentRoot->numberChildren; i++) {
        *(parentRoot->children + i) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
        memcpy(*(parentRoot->children + i), *(temp + i), sizeof(struct trie_node_t));
        free(*(temp + i));
      }
      *(parentRoot->children + parentRoot->numberChildren) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
      memcpy(*(parentRoot->children + parentRoot->numberChildren), insertpos, sizeof(struct trie_node_t));
      parentRoot->numberChildren++;
    }
  } else {
    if(iterations == 0) {
      // Otherwise if this element has no subkey, it must instead
      // expand from the root -- this code expands adds one value to the root
      // and assigns that new value to the first value in the current key
      trie_pos_t fillerPos = malloc(sizeof(struct trie_node_t));
      fillerPos->data = NULL;
      fillerPos->key = *str;
      fillerPos->numberChildren = 0;
      fillerPos->children = NULL;
      if(trie->numberRootPaths != 0) {
        trie_pos_t * temp = (trie_pos_t *) malloc(sizeof(trie_pos_t) * trie->numberRootPaths);
        for(int i = 0; i < trie->numberRootPaths; i++) {
          *(temp + i) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
          memcpy(*(temp + i), *(trie->root + i), sizeof(struct trie_node_t));
          free(*(trie->root + i));
        }
        trie->root = (trie_pos_t *) malloc(sizeof(trie_pos_t) * (trie->numberRootPaths + 1));
        for(int i = 0; i < (trie->numberRootPaths + 1); i++) {
          if(i < trie->numberRootPaths) {
            *(trie->root + i) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
            memcpy(*(trie->root + i), *(temp + i), sizeof(struct trie_node_t));
            free(*(temp + i));
          } else {
            *(trie->root + i) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
            memcpy(*(trie->root + i), fillerPos, sizeof(struct trie_node_t));
          }
        }
        free(temp);
      } else {
        trie->root = (trie_pos_t *) malloc(sizeof(trie_pos_t));
        *(trie->root) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
        memcpy(*(trie->root), fillerPos, sizeof(struct trie_node_t));
      }
      free(fillerPos);
      trie->numberRootPaths++;

      iterations++;
    }

    // This function iterates over the remaining gap between
    // the first value we created expanding from the root, and the current
    // key -- creating a subtree that expands outwards
    for(int i = 1; i < strlen(str); i++) {
      char newKey[iterations + i + 1];
      memcpy(newKey, str, iterations + i + 1);
      newKey[iterations + i] = '\0';
      trie_insert_recurse(trie, newKey, NULL, NULL);
    }
  }

  if(newpos)
    *newpos = insertpos;
  return true;
}

// Better implementation of this function would've been to write seperate
// wrapper and recursive functions from the start
bool trie_insert(trie_t trie, const char * str, void * newval, trie_pos_t * newpos) {
  bool returnStatus = trie_insert_recurse(trie, str, newval, newpos);
  if(returnStatus) {
    char ** temp = (char **) malloc(sizeof(char *) * trie->size);
    // Copy all values into a temp pointer
    for(int i = 0; i < trie->size; i++) {
      *(temp + i) = (char *) malloc(sizeof(char) * (strlen(*(trie->keys + i)) + 1));
      memcpy(*(temp + i), *(trie->keys + i), sizeof(char) * (strlen(*(trie->keys + i)) + 1));
      free(*(trie->keys + i));
    }
    if(trie->keys)
      free(trie->keys);
    // Reallocate a pointer that is one larger in value
    trie->keys = (char **) malloc(sizeof(char *) * (trie->size + 1));
    // Copy all values in temp back over into the keys pointer
    for(int i = 0; i < trie->size; i++) {
      *(trie->keys + i) = (char *) malloc(sizeof(char) * (strlen(*(temp + i)) + 1));
      memcpy(*(trie->keys + i), *(temp + i), sizeof(char) * (strlen(*(temp + i)) + 1));
      free(*(temp + i));
    }
    free(temp);
    // Copy over the final new key values into keys
    *(trie->keys + (trie->size)) = (char *) malloc(sizeof(char) * (strlen(str) + 1));
    memcpy(*(trie->keys + (trie->size++)), str, sizeof(char) * (strlen(str) + 1));
    return true;
  }
  return false;
}

bool trie_remove (trie_t trie, const char * key, void ** data) {
  // Check to see if a certain key exists
  // or if that key is just a subkey
  int found = 0;
  for(int i = 0; i < trie->size; i++) {
    if(*(trie->keys + i) == key)
      found = 1;
  }
  if(found == 0)
    return false;

  trie_pos_t removePos;
  if((removePos = trie_find(trie, key)) == TRIE_INVALID_POS) {
    return false;
  }

  // Sets data to be the position's data only if it is not NULL
  if(removePos->data) {
    *data = removePos->data;
  } else {
    data = NULL;
  }

  // Finds the subkey that will locate the parent
  char newKey[strlen(key)];
  memcpy(newKey, key, strlen(key));
  newKey[strlen(key) - 1] = '\0';
  trie_pos_t parent;
  if((parent = trie_find(trie, key)) == TRIE_INVALID_POS)
    return false;

  // Copy all of the children of the parent into a temp value
  trie_pos_t * temp = (trie_pos_t *) malloc(sizeof(trie_pos_t) * parent->numberChildren);
  for(int i = 0; i < parent->numberChildren; i++) {
    *(temp + i) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
    memcpy(*(temp + i), *(parent->children + i), sizeof(struct trie_node_t));
    free(*(parent->children + i));
  }
  // Reallocate the parent children pointer to account for appended children
  parent->children = (trie_pos_t *) malloc(sizeof(trie_pos_t) * (parent->numberChildren + removePos->numberChildren));
  // Copy all the values of the temp pointer back into the parent pointer
  for(int i = 0; i < (parent->numberChildren + removePos->numberChildren); i++) {
    if(i < parent->numberChildren) {
      *(parent->children + i) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
      memcpy(*(parent->children + i), *(temp + i), sizeof(struct trie_node_t));
    } else {
      // Copy all the new value pointer back into the parent pointer
      *(parent->children + i) = (trie_pos_t) malloc(sizeof(struct trie_node_t));
      memcpy(*(parent->children + i), *(removePos->children + (i - parent->numberChildren)), sizeof(struct trie_node_t));
    }
  }

  parent->numberChildren += removePos->numberChildren;

  free(temp);

  return true;
}

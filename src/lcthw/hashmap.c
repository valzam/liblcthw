#undef NDEBUG
#include <lcthw/bstrlib.h>
#include <lcthw/dbg.h>
#include <lcthw/hashmap.h>
#include <stdint.h>

static int default_compare(void *a, void *b) {
  return bstrcmp((bstring)a, (bstring)b);
}

/**
 * Bob jenkins's hash algorithm
 */

static uint32_t default_hash(void *a) {
  size_t len = blength((bstring)a);
  char *key = bdata((bstring)a);
  uint32_t hash = 0;
  uint32_t i = 0;

  for (hash = i = 0; i < len; ++i) {
    hash += key[i];
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }

  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);

  return hash;
}

Hashmap *Hashmap_create(Hashmap_compare compare, Hashmap_hash hash) {
  Hashmap *map = calloc(1, sizeof(Hashmap));
  check_mem(map);

  map->compare = compare == NULL ? default_compare : compare;
  map->hash = hash == NULL ? default_hash : hash;
  map->buckets = DArray_create(sizeof(DArray *), DEFAULT_NUMBER_OF_BUCKETS);
  check_mem(map->buckets);

  map->buckets->end = map->buckets->max;

  return map;

error:
  if (map) {
    Hashmap_destroy(map);
  }
  return NULL;
}

void Hashmap_destroy(Hashmap * map) {
  int i = 0;
  int j = 0;

  if (map) {
    if (map->buckets) {
      for (i = 0; i < DArray_count(map->buckets); i++) {
        DArray *bucket = DArray_get(map->buckets, i);
        if (bucket) {
          for (j = 0; j < DArray_count(bucket); j++) {
            free(DArray_get(bucket, j));
          }
          DArray_destroy(bucket);
        }
      }
      DArray_destroy(map->buckets);
    }
    free(map);
  }
}

static inline HashmapNode *Hashmap_node_create(int hash, void *key, void *data) {
  HashmapNode *node = calloc(1, sizeof(HashmapNode));
  check_mem(node);
  
  node->key = key;
  node->data = data;
  node->hash = hash;

  return node;

error:
  return NULL;
}

static inline DArray *Hashmap_find_bucket(Hashmap * map, void *key,
int create, uint32_t * hash_out) {
  uint32_t hash = map->hash(key);
  int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
  check(bucket_n >= 0, "Invlaid bucket found: %d", bucket_n);
  // store hash so that caller can check
  *hash_out = hash;

  DArray *bucket = DArray_get(map->buckets, bucket_n);

  // make this function dual purpose:
  // If create is 1 then we also create a bucket for the key
  // e.g. when assigning to the map
  if (!bucket && create) {
    // We reuse default max number of bucket as default max number of bucket size
    bucket = DArray_create(sizeof(void *), DEFAULT_NUMBER_OF_BUCKETS);
    check_mem(bucket);
    DArray_set(map->buckets, bucket_n, bucket);
  }

  return bucket;

error:
  return NULL;
}

static inline HashmapNode * Hashmap_get_node(Hashmap *map, uint32_t hash, DArray * bucket, void *key) {
  int i = 0;

  for (i = 0; i < DArray_end(bucket); i++) {
    debug("TRY: %d", i);
    HashmapNode *node = DArray_get(bucket, i);
    check(node != NULL, "Could not get node even though it should exist");

    if (node->hash == hash && map->compare(node->key, key) == 0) {
      return node;
    }
  }

  return NULL;

error:
  return NULL;
}

int Hashmap_set(Hashmap *map, void *key, void *data) {
  // We initialize the hash here
  // When we try to find the appropriate bucket for the key the
  // Hashmap_find_bucket function calculates the hash and stores it in this
  // variable. We can then use 'hash' to create the HashmapNode and store it in
  // the appropriate bucket
  uint32_t hash = 0;
  DArray *bucket = Hashmap_find_bucket(map, key, 1, &hash);
  check(bucket, "Error can't create bucket.");

  // Check if already exists
  HashmapNode *node = Hashmap_get_node(map, hash, bucket, key);
  if (!node) {
    // Doesn't exist, create new node and add it
    node = Hashmap_node_create(hash, key, data);
    check_mem(node);

    DArray_push(bucket, node);
  } else {
    // key already exists, overwrite data
    debug("Key already exists, overwritting");
    node->data = data;
  }

error:
  return -1;
}

void *Hashmap_get(Hashmap * map, void *key) {
  // 1. Find appropriate bucket for key
  // 2. Traverse bucket to find location of key
  // 3. Return data associated with key
  uint32_t hash = 0;

  DArray *bucket = Hashmap_find_bucket(map, key, 0, &hash);
  if (!bucket) return NULL;

  HashmapNode *node = Hashmap_get_node(map, hash, bucket, key);
  if (!node) return NULL;

  return node->data;
}

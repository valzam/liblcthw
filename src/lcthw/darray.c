#include <lcthw/darray.h>
#include <lcthw/dbg.h>
#include <stdlib.h>

DArray *DArray_create(size_t element_size, size_t initial_max) {
  DArray *array = calloc(1, sizeof(DArray));
  check_mem(array);

  // Initialize array
  array->element_size = element_size;
  array->max = initial_max;
  array->end = 0;
  array->expand_rate = DEFAULT_EXPAND_RATE;
  array->contents = calloc(initial_max, sizeof(void *));
  check_mem(array->contents);

  return array;

error:
  return NULL;
}

void DArray_destroy(DArray *array) { free(array); }

void DArray_clear(DArray *array) {
  int i = 0;
  for (i = 0; i < array->max; i++) {
    free(array->contents[i]);
  }
}

void *DArray_clear_destroy(DArray *array) {
  DArray_clear(array);
  DArray_destroy(array);
}
static inline int DArray_resize(DArray *array, size_t newsize) {
  array->max = newsize;
  check(array->max > 0, "The newsize must be > 0.");

  void *contents = realloc(array->contents, array->max * sizeof(void *));
  // check contents and assume realloc doesn't harm the original on error

  check_mem(contents);

  array->contents = contents;

  return 0;
error:
  return -1;
}

void DArray_expand(DArray *array) {
  check(array != NULL, "darray trying to expand uninitialized array");
  size_t old_max = array->max;

  // Resize array and copy over contents from previous memor block
  DArray_resize(array, array->element_size * array->expand_rate);

  // Zero out old memory area
  memset(array->contents + old_max, 0, array->expand_rate + 1);

  return 0;

error:
  return NULL;
}

void DArray_contract(DArray *array) {}

int DArray_push(DArray *array, void *el) { 
  

  return 1; 
}

void *DArray_pop(DArray *array) { return NULL; }

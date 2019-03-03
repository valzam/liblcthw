#include <assert.h>
#include <lcthw/list.h>
#include "minunit.h"

static List *list = NULL;
static List *second_list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create() {
  list = List_create();
  second_list = List_create();
  mu_assert(list != NULL, "Failed to create list.");

  return NULL;
}

char *test_destroy() {
  List_clear_destroy(list);

  return NULL;
}

char *test_push_pop() {
  List_push(list, test1);
  mu_assert(List_last(list) == test1, "Wrong last value.");

  List_push(list, test2);
  mu_assert(List_last(list) == test2, "Wrong last value");

  List_push(list, test3);
  mu_assert(List_last(list) == test3, "Wrong last value.");
  mu_assert(List_length(list) == 3, "Wrong count on push.");

  char *val = List_pop(list);
  mu_assert(val == test3, "Wrong value on pop.");

  val = List_pop(list);
  mu_assert(val == test2, "Wrong value on pop.");

  val = List_pop(list);
  mu_assert(val == test1, "Wrong value on pop.");
  mu_assert(List_length(list) == 0, "Wrong count after pop.");

  return NULL;
}

char *test_unshift() {
  List_unshift(list, test1);
  mu_assert(List_first(list) == test1, "Wrong first value.");

  List_unshift(list, test2);
  mu_assert(List_first(list) == test2, "Wrong first value");

  List_unshift(list, test3);
  mu_assert(List_first(list) == test3, "Wrong last value.");
  mu_assert(List_length(list) == 3, "Wrong count on unshift.");

  return NULL;
}

char *test_remove() {
  // we only need to test the middle remove case since push/shift
  // already tests the other cases

  char *val = List_remove(list, list->first->next);
  mu_assert(val == test2, "Wrong removed element.");
  mu_assert(List_length(list) == 2, "Wrong count after remove.");
  mu_assert(List_first(list) == test3, "Wrong first after remove.");
  mu_assert(List_last(list) == test1, "Wrong last after remove.");

  return NULL;
}

char *test_shift() {
  mu_assert(List_length(list) != 0, "Wrong count before shift.");

  char *val = List_shift(list);
  mu_assert(val == test3, "Wrong value on shift.");

  val = List_shift(list);
  mu_assert(val == test1, "Wrong value on shift.");
  mu_assert(List_length(list) == 0, "Wrong count after shift.");

  return NULL;
}

char *test_join() {
  List *list = List_create();
  List *second_list = List_create();

  List_push(list, test1);
  List_push(second_list, test2);
  
  List_join(list, second_list);

  ListNode *result = list->first->next;

  mu_assert(result->value == test2, "Lists not joined properly");
  mu_assert(list->length == 2, "Counter not updated properly");
  return NULL;
}

char *test_copy() {
  List_push(list, test1);
  List_push(list, test2);
  List_push(list, test3);

  List *new_list = List_copy(list);

  mu_assert(new_list->first->value == list->first->value, "Wrong value on first node");
  mu_assert(new_list->first->next->value == list->first->next->value, "Wrong value on middle node");
  mu_assert(new_list->last->value == list->last->value, "Wrong value on last node");
  return NULL;
}

char *test_split() {
  List *list = List_create();
  List_push(list, test1);
  List_push(list, test2);
  List_push(list, test3);
  int position = 2;
  List *left = List_create();
  List *right = List_create();

  List_split(list, position, left, right);

  mu_assert(left->first->value == test1, "Wrong value on first left node");
  mu_assert(left->first->next->value == test2, "Wrong value on last left node");
  mu_assert(right->first->value == test3, "Wrong value on first right node");
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_push_pop);
  mu_run_test(test_unshift);
  mu_run_test(test_remove);
  mu_run_test(test_shift);
  mu_run_test(test_destroy);
  mu_run_test(test_join);
  mu_run_test(test_copy);
  mu_run_test(test_split);

  return NULL;
}

RUN_TESTS(all_tests);

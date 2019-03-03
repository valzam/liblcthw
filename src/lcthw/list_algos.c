#include <lcthw/dbg.h>
#include <lcthw/list.h>
#include <lcthw/list_algos.h>

int swap(ListNode *left, ListNode *right) {
  // Cache the node after right
  ListNode *tmp = right->next;

  // Put right in left's place
  right->prev = left->prev;
  right->next = left;

  // Check for when we switch first and second element
  if (left->prev != NULL) {
    left->prev->next = right;
  }
  // Put left in right's place
  left->prev = right;
  left->next = tmp;

  return 1;
}

int left_bigger_right(ListNode *left, ListNode *right, List_compare cmp) {
  check(left != NULL && right != NULL,
        "Cannot pass NULL nodes to compare function");
  check(left->value != NULL && right->value != NULL,
        "Cannot pass nodes with NULL values to compare");

  return cmp(left->value, right->value) > 0 ? 1 : 0;

error:
  exit(1);
}

/*
procedure bubbleSort( A : list of sortable items )
    n = length(A)
    repeat
        swapped = false
        for i = 1 to n-1 inclusive do
            if A[i] > A[i+1] then
                swap(A[i], A[i+1])
                swapped = true
            end if
        end for
        n = n - 1
    until not swapped
end procedure
*/

int List_bubble_sort(List *list, List_compare cmp) {
  int swapped = 0;
  int start_of_tail = list->length;
  int position_in_list = 0;

  check(list, "Passed NULL value for list");

  do {
    // reset indicator variables
    swapped = 0;
    position_in_list = 0;

    LIST_FOREACH(list, first, next, cur) {
      // reached sorted tail
      if (position_in_list >= start_of_tail || cur->next == NULL) {
        break;
      }

      ListNode *right = cur->next;
      if (left_bigger_right(cur, right, cmp)) {
        swapped = swap(cur, cur->next);
      }

      position_in_list++;
    }
    // By definition one more item at the tail of the list will be sorted
    // Bubble sort "bubbles" the largest value in the head of the list to the
    // end
    start_of_tail--;
  } while (swapped);

  return 0;

error:
  return 1;
}

inline List *List_merge(List *left, List *right, List_compare cmp) {
  List *result = List_create();
  void *val = NULL;

  while (List_length(left) > 0 || List_length(right) > 0) {
    if (List_length(left) > 0 && List_length(right) > 0) {
      if (left_bigger_right(left->first, right->first, cmp)) {
        val = List_shift(right);
      } else {
        val = List_shift(left);
      }

      List_push(result, val);
    } else if (List_length(left) > 0) {
      val = List_shift(left);
      List_push(result, val);
    } else if (List_length(right) > 0) {
      val = List_shift(right);
      List_push(result, val);
    }
  }

  return result;
}

List *List_merge_sort(List *list, List_compare cmp) {
  check(list != NULL, "Passed invalid NULL list to merge_sort");

  List *result = NULL;

  if (List_length(list) <= 1) {
    return list;
  }

  List *left = List_create();
  List *right = List_create();
  check_mem(left);
  check_mem(right);

  int middle = List_length(list) / 2;
  List_split(list, middle, left, right);

  List *sort_left = List_merge_sort(left, cmp);
  List *sort_right = List_merge_sort(right, cmp);

  if (sort_left != left) List_destroy(left);
  if (sort_right != right) List_destroy(right);

  result = List_merge(sort_left, sort_right, cmp);

  List_destroy(sort_left);
  List_destroy(sort_right);

  return result;

error:
  return list;
}

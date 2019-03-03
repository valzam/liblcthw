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
  return cmp(left->value, right->value) > 0 ? 1 : 0;
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
  int start_of_tail = list->count;
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
  // Bubble sort "bubbles" the largest value in the head of the list to the end
  start_of_tail--;
  } while (swapped);

  return 0;

error:
  return 1;
}

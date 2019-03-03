#include <lcthw/list.h>
#include <lcthw/dbg.h>

List *List_create() {
    List *list = calloc(1, sizeof(List));
    list->count = -1;
    
    return list;
}

void List_destroy(List * list) {

    LIST_FOREACH(list, first, next, cur) {
        if (cur->prev) {
            free(cur->prev);
        }
    }

    free(list->last);
    free(list);
}

void List_clear(List * list) {
    LIST_FOREACH(list, first, next, cur) {
        if (cur->value) {
            free(cur->value);
        }
    }
}

void List_clear_destroy(List * list) {
    List_clear(list);
    List_destroy(list);
}

void List_push(List * list, void *value) {
    ListNode *new_node = calloc(1, sizeof(ListNode));
    check_mem(new_node);

    new_node->value = value;

    if (list->last == NULL) {
        list->first = new_node;
        list->last = new_node;
    } else {
      list->last->next = new_node;
      new_node->prev = list->last;
      list->last = new_node;
    }

    list->count++;
    
error:
    return;
}

void *List_pop(List * list) {
    ListNode *node = list->last;
    return node != NULL ? List_remove(list, node) : NULL;
}

void List_unshift(List * list, void *value) {
    ListNode *new_node = calloc(1, sizeof(ListNode));
    check_mem(new_node);

    new_node->value = value;

    if (list->first == NULL) {
        list->first = new_node;
        list->last = new_node;
    } else {
        list->first->prev = new_node;
        new_node->next = list->first;
        list->first = new_node;
    }
    list->count++;

error:
    return;
}

void *List_shift(List * list) {
    ListNode *node = list->first;
    return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List * list, ListNode * node) {
    void *result = NULL;

    check(list->first && list->last, "List is empty");
    check(node, "Node cannot be null");

    // List only has one element
    if (node == list->first && node == list->last) {
        list->first = NULL;
        list->last = NULL;
    // Node is the first element
    } else if (node == list->first) {
        list->first = list->first->next;
    // Node is the last element
    } else if (node == list->last) {
        list->last = list->last->prev;
    // Node is somewhere in the middle
    } else {
        ListNode *before = node->prev;
        ListNode *after = node->next;

        before->next = after;
        after->prev = before;
    }

    list->count--;
    result = node->value;
    free(node);
error:
    return result;
}

void List_join(List *root, List *addition) {
    check(root->first != NULL && addition->first, "One of the lists was empty");

    root->last->next = addition->first;
    addition->first->prev = root->last;
    root->count += addition->count + 1;

error:
    return;
}

List *List_copy(List * list) {
    check(list->first != NULL, "Trying to copy empty list");

    List *new_list = List_create();

    LIST_FOREACH(list, first, next, cur) {
        List_push(new_list, cur->value);
    }

    return new_list;

error:
    return List_create();
}

void List_split(List *list, int position, List *left, List *right) {
    check(list, "List cannot be null null");
    check(list->count >= 0, "Cannot split a list with only one node");
    check(left->first == NULL && right->first == NULL, "Please pass empty tager lists for splitting");

    /*
    No copy:
    Option 1:
        Iterate in list until node is found
        - list->first, node, left_counter to left
        - node->next, list->last, right_counter to right
    Option 2:
        treat list as left output, repoint list->last to node and node->next, list->last
    
    With copy:
        Option 1:
        Find node at position, 
    Should this function free list? Probably better left to caller
    */
   int counter = 0;
   LIST_FOREACH(list, first, next, cur) {
       if (counter <= position) {
           List_push(left, cur->value);
       } else {
           List_push(right, cur->value);
       }

       counter++;
   }

error:
    return;
}

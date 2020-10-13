#include "tasklist.h"
#include <stdlib.h>

void add_task(const Task *task, TaskList *list) {
  TaskNode *node = malloc(sizeof(TaskNode));
  if (node == NULL) {
    exit(1);
  }
  node->task = *task;
  node->next = *list;
  *list = node;
}

void add_sorted_task(const Task *task, TaskList *list, int (*comparator)(const Task *, const Task *)) {
  TaskNode *insert_after = NULL, *insert_before = *list;
  TaskNode *node;

  while (insert_before != NULL) {
    if (comparator(task, &(insert_before)->task) < 0) {
      break;
    }
    insert_after = insert_before;
    insert_before = insert_before->next;
  }
  node = insert_before;
  add_task(task, &node);
  if (insert_after == NULL) {
    *list = node;
  } else {
    insert_after->next = node;
  }
}

int count_task_list(TaskList list) {
  int c = 0;
  while (list != NULL) {
    c++;
    list = list->next;
  }
  return c;
}

void print_task_list(TaskList list) {
  while (list != NULL) {
    print_task(&list->task);
    list = list->next;
  }
}

void free_list(TaskList list) {
  TaskNode *to_free;
  while (list != NULL) {
    to_free = list;
    list = list->next;
    free(to_free);
  }
}

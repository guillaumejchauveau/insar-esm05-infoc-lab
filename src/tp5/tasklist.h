#ifndef C_LAB_TASKLIST_H
#define C_LAB_TASKLIST_H

#include "task.h"

typedef struct task_node {
  Task task;
  struct task_node *next;
} TaskNode;

typedef TaskNode *TaskList;

void add_task(const Task *task, TaskList *list);
void add_sorted_task(const Task *task, TaskList *list, int (*comparator)(const Task *, const Task *));
int count_task_list(TaskList list);
void print_task_list(TaskList list);
void free_list(TaskList list);

#endif

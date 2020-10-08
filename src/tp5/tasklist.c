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

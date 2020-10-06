#ifndef TP4_TASK_H
#define TP4_TASK_H

#include <limits.h>
#include <stdio.h>

#define MAXTASKS 64
#define MAXDEPS 16
#define MAXTITLE 64

typedef struct {
  int id;
  int duration;
  int dependencies[MAXDEPS];
  int dependencies_count;
  char title[MAXTITLE];
} Task;

void prompt_task(Task *task);
void print_task(const Task *task);
int load_task(FILE *file, Task *task);
int load_tasks(FILE *file, Task *task_array, int max_task_array_size);

#endif

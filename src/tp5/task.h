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
void print_tasks(const Task *tasks, int tasks_count);
int tasks_total_duration(const Task *tasks, int tasks_count);
int load_task(FILE *file, Task *task);
int load_tasks(FILE *file, Task **task_array);
int save_task(FILE *file, const Task *task);
int save_tasks(FILE *file, const Task *tasks, int tasks_count);

#endif

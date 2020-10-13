#include "task.h"
#include <string.h>
#include "tasklist.h"
#include <stdlib.h>

int id_comparator(const Task *t1, const Task *t2) {
  return t1->id - t2->id;
}

int title_comparator(const Task *t1, const Task *t2) {
  return strcmp(t1->title, t2->title);
}

int main() {
  Task *tasks;
  FILE *task_file;
  int task_count;
  TaskList list = NULL;
  int i;

  task_file = fopen("../../src/tp5/tasks.txt", "r");
  if (task_file == NULL) {
    perror("Error opening file");
    return 1;
  }

  task_count = load_tasks(task_file, &tasks);
  for (i = 0; i < task_count; i++) {
    add_sorted_task(tasks + i, &list, &title_comparator);
  }

  if (fclose(task_file) == EOF) {
    fprintf(stderr, "Error closing file.\n");
    return 1;
  }

  print_task_list(list);

  free(tasks);
  free_list(list);

  return 0;
}

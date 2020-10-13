#include "task.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void prompt_task(Task *task) {
  int deps = 0;
  printf("Enter task info:\n");
  do {
    printf("ID: ");
    scanf("%d", &task->id);
  } while (0);
  do {
    printf("Duration: ");
    scanf("%d", &task->duration);
  } while (task->duration < 0);

  do {
    printf("Title: ");
    scanf("%s", task->title);
  } while (0);

  do {
    printf("Dependency count: ");
    scanf("%d", &task->dependencies_count);
  } while (task->dependencies_count > MAXDEPS ||
      task->dependencies_count < 0);

  while (deps < task->dependencies_count) {
    printf("Dependency %d: ", deps);
    scanf("%d", task->dependencies + deps);
  }
}

void print_task(const Task *task) {
  printf("Task \"%s\":\n", task->title);
  printf("ID: %d\n", task->id);
  printf("Duration: %d\n", task->duration);
  printf("Has %d dependencies\n", task->dependencies_count);
}

void print_tasks(const Task *tasks, int tasks_count) {
  int i;
  for (i = 0; i < tasks_count; i++) {
    print_task(tasks + i);
  }
}

int tasks_total_duration(const Task *tasks, int tasks_count) {
  int i, total = 0;
  for (i = 0; i < tasks_count; i++) {
    total += tasks[i].duration;
  }
  return total;
}

typedef enum {
  ID,
  DURATION,
  DEPENDENCIES_COUNT,
  DEPENDENCY,
  TITLE
} LOAD_STATE;

int load_task(FILE *file, Task *task) {
  char token[MAXTITLE];
  int token_size = 0;
  int c;
  LOAD_STATE state = ID;
  int dependencies_count = 0;

  while (1) {
    c = getc(file);
    if (c != '\n' && isspace(c)) {
      if (token_size == 0) {
        continue;
      }
      token[token_size] = 0;

      switch (state) {
        case ID:
          task->id = strtol(token, NULL, 10);
          state = DURATION;
          break;
        case DURATION:
          task->duration = strtol(token, NULL, 10);
          state = DEPENDENCIES_COUNT;
          break;
        case DEPENDENCIES_COUNT:
          task->dependencies_count = strtol(token, NULL, 10);
          state = task->dependencies_count == 0 ? TITLE : DEPENDENCY;
          break;
        case DEPENDENCY:
          task->dependencies[dependencies_count] = strtol(token, NULL, 10);

          if (++dependencies_count == task->dependencies_count) {
            state = TITLE;
          }
          break;
        case TITLE:
          token[token_size++] = (char) c;
          continue;
        default:
          return -1;
      }
      token_size = 0;
      continue;
    }

    if (c == '\n' || c == EOF) {
      switch (state) {
        case TITLE:
          strncpy(task->title, token, token_size);
          task->title[token_size] = 0;
          return 1;
        case ID:
          return 0;
        default:
          return -1;
      }
    }
    token[token_size++] = (char) c;
  }
}

int load_tasks(FILE *file, Task **task_array) {
  int loaded_task_count = 0;
  int max_task_array_size, res;
  fscanf(file, "%d\n", &max_task_array_size);
  *task_array = malloc(max_task_array_size * sizeof(Task));

  while (loaded_task_count < max_task_array_size && !feof(file)) {
    res = load_task(file, (*task_array) + loaded_task_count);
    if (res == -1) {
      return -1;
    }
    loaded_task_count += res;
  }
  return loaded_task_count;
}

int save_task(FILE *file, const Task *task) {
  int i;
  if (fprintf(file, "%d %d %d ", task->id, task->duration, task->dependencies_count) < 0) {
    return 0;
  }
  for (i = 0; i < task->dependencies_count; i++) {
    if (fprintf(file, "%d ", task->dependencies[i]) < 0) {
      return 0;
    }
  }
  if (fprintf(file, "%s\n", task->title) < 0) {
    return 0;
  }

  return 1;
}

int save_tasks(FILE *file, const Task *tasks, int tasks_count) {
  int i;
  if (fprintf(file, "%d\n", tasks_count) < 0) {
    return 0;
  }
  for (i = 0; i < tasks_count; i++) {
    if (save_task(file, tasks + i) == 0) {
      return 0;
    }
  }
  return 1;
}

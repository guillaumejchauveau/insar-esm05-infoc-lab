#include "task.h"
#include <string.h>
#include "tasklist.h"
#include <stdlib.h>

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
    for (i=0; i < task_count; i++) {
        add_task(tasks + i, &list);
    }

    if (fclose(task_file) == EOF) {
        fprintf(stderr, "Error closing file.\n");
        return 1;
    }

    print_task_list(list);

/*
    task_file = fopen("../../src/tp5/tasksB.txt", "w");
    if (task_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    save_tasks(task_file, tasks, task_count);

    if (fclose(task_file) == EOF) {
        fprintf(stderr, "Error closing file.\n");
        return 1;
    }
*/
    free(tasks);
    free_list(list);

    return 0;
}

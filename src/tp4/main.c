#include "task.h"
#include <string.h>

int main() {
    Task tasks[MAXTASKS];
    FILE *task_file;
    int task_count;

    task_file = fopen("../../src/tp4/tasks.txt", "r");
    if (task_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    task_count = load_tasks(task_file, tasks, MAXTASKS);

    if (fclose(task_file) == EOF) {
        fprintf(stderr, "Error closing file.\n");
        return 1;
    }

    print_tasks(tasks, task_count);


    task_file = fopen("../../src/tp4/tasksB.txt", "w");
    if (task_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    save_tasks(task_file, tasks, task_count);

    if (fclose(task_file) == EOF) {
        fprintf(stderr, "Error closing file.\n");
        return 1;
    }

    return 0;
}

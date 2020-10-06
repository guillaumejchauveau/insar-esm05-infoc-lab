#include "task.h"
#include <string.h>

int main() {
    Task tasks[MAXTASKS];
    FILE *task_file;
    int task_count = 0;

    task_file = fopen("../tasks.txt", "r");
    if (task_file == NULL) {
        perror("Error opening file");
        return 1;
    }

    load_task(task_file, tasks);

    task_count = load_tasks(task_file, tasks, MAXTASKS);

    if (fclose(task_file) == EOF) {
        fprintf(stderr, "Error closing file.\n");
        return 1;
    }

    return 0;
}

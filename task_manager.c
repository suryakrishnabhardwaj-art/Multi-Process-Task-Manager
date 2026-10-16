#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <signal.h>
#include <unistd.h>

#define MAX_PROCESSES 100

void clear_screen() {
    printf("\033[2J\033[H");
}

int is_number(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit((unsigned char)str[i]))
            return 0;
    }
    return 1;
}

void get_process_name(int pid, char *name, size_t size) {
    char path[64];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%d/comm", pid);

    file = fopen(path, "r");

    if (file == NULL) {
        snprintf(name, size, "Unknown");
        return;
    }

    if (fgets(name, size, file) != NULL) {
        name[strcspn(name, "\n")] = '\0';
    } else {
        snprintf(name, size, "Unknown");
    }

    fclose(file);
}

char get_process_state(int pid) {
    char path[64];
    FILE *file;
    char line[512];
    char state = '?';

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    file = fopen(path, "r");

    if (file == NULL)
        return '?';

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "State:", 6) == 0) {
            sscanf(line, "State:\t%c", &state);
            break;
        }
    }

    fclose(file);
    return state;
}

long get_memory_kb(int pid) {
    char path[64];
    FILE *file;
    char line[512];
    long memory = 0;

    snprintf(path, sizeof(path), "/proc/%d/status", pid);

    file = fopen(path, "r");

    if (file == NULL)
        return 0;

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "VmRSS:", 6) == 0) {
            sscanf(line, "VmRSS: %ld", &memory);
            break;
        }
    }

    fclose(file);
    return memory;
}

void display_processes() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("/proc");

    if (dir == NULL) {
        perror("Unable to open /proc");
        return;
    }

    printf("\n%-8s %-8s %-12s %-30s\n",
           "PID", "STATE", "MEMORY", "PROCESS");

    printf("----------------------------------------------------------------\n");

    while ((entry = readdir(dir)) != NULL) {

        if (!is_number(entry->d_name))
            continue;

        int pid = atoi(entry->d_name);

        char name[256];
        char state;
        long memory;

        get_process_name(pid, name, sizeof(name));
        state = get_process_state(pid);
        memory = get_memory_kb(pid);

        printf("%-8d %-8c %-8ld KB   %-30s\n",
               pid, state, memory, name);
    }

    closedir(dir);
}

void view_process(int pid) {
    char name[256];
    char state;
    long memory;

    get_process_name(pid, name, sizeof(name));
    state = get_process_state(pid);
    memory = get_memory_kb(pid);

    printf("\n========== PROCESS INFORMATION ==========\n");
    printf("PID       : %d\n", pid);
    printf("Name      : %s\n", name);
    printf("State     : %c\n", state);
    printf("Memory    : %ld KB\n", memory);
    printf("=========================================\n");
}

int is_user_owned_process(int pid) {
    char path[64];
    char line[512];
    FILE *file;
    unsigned int process_uid;

    snprintf(path, sizeof(path), "/proc/%d/status", pid);
    file = fopen(path, "r");

    if (file == NULL)
        return 0;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, "Uid:", 4) == 0) {
            if (sscanf(line, "Uid: %u", &process_uid) == 1) {
                fclose(file);
                return process_uid == (unsigned int)getuid();
            }
            break;
        }
    }

    fclose(file);
    return 0;
}

void terminate_process(int pid) {
    if (pid <= 1) {
        printf("\n[WARNING] System PID cannot be terminated.\n");
        return;
    }

    if (kill(pid, SIGTERM) == 0)
        printf("\n[SUCCESS] SIGTERM sent to PID %d.\n", pid);
    else
        perror("\n[ERROR] Unable to terminate process");
}

void stop_process(int pid) {
    if (pid <= 1) {
        printf("\n[WARNING] System PID cannot be stopped.\n");
        return;
    }

    if (kill(pid, SIGSTOP) == 0)
        printf("\n[SUCCESS] Process %d stopped.\n", pid);
    else
        perror("\n[ERROR] Unable to stop process");
}

void continue_process(int pid) {
    if (pid <= 1) {
        printf("\n[WARNING] System PID cannot be controlled.\n");
        return;
    }

    if (kill(pid, SIGCONT) == 0)
        printf("\n[SUCCESS] Process %d continued.\n", pid);
    else
        perror("\n[ERROR] Unable to continue process");
}

void process_control() {
    int pid;
    int choice;

    printf("\nEnter PID: ");
    if (scanf("%d", &pid) != 1) {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
        printf("[ERROR] Invalid PID.\n");
        return;
    }

    if (pid <= 1 || pid == getpid()) {
        printf("[WARNING] System PIDs and this manager are protected.\n");
        return;
    }

    if (kill(pid, 0) != 0) {
        perror("[ERROR] Process does not exist or permission denied");
        return;
    }

    if (!is_user_owned_process(pid)) {
        printf("[WARNING] Only processes owned by your user can be controlled.\n");
        return;
    }

    while (1) {

        printf("\n========== PROCESS CONTROL ==========\n");
        printf("PID: %d\n", pid);
        printf("-------------------------------------\n");
        printf("1. View Process Information\n");
        printf("2. Terminate Process (SIGTERM)\n");
        printf("3. Stop Process (SIGSTOP)\n");
        printf("4. Continue Process (SIGCONT)\n");
        printf("5. Back to Main Menu\n");
        printf("=====================================\n");

        printf("Enter choice: ");
        if (scanf("%d", &choice) != 1) {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {}
            printf("\nInvalid choice.\n");
            return;
        }

        switch (choice) {

            case 1:
                view_process(pid);
                break;

            case 2: {
                char confirm;
                printf("\nConfirm termination of PID %d? (y/N): ", pid);

                if (scanf(" %c", &confirm) != 1) {
                    printf("\n[ERROR] Could not read confirmation.\n");
                    return;
                }

                if (confirm == 'y' || confirm == 'Y') {
                    terminate_process(pid);
                    return;
                }

                printf("\n[CANCELLED] Termination cancelled.\n");
                break;
            }

            case 3:
                stop_process(pid);
                break;

            case 4:
                continue_process(pid);
                break;

            case 5:
                return;

            default:
                printf("\nInvalid choice.\n");
        }
    }
}

void display_menu() {
    printf("\n===============================================\n");
    printf("        MULTI-PROCESS TASK MANAGER\n");
    printf("===============================================\n");
    printf("1. Display All Processes\n");
    printf("2. Control Selected Process\n");
    printf("3. Exit\n");
    printf("===============================================\n");
    printf("Enter choice: ");
}

int main() {

    int choice;

    while (1) {

        clear_screen();

        printf("===============================================\n");
        printf("        MULTI-PROCESS TASK MANAGER\n");
        printf("              FOR LINUX\n");
        printf("===============================================\n");

        printf("Manager PID: %d\n", getpid());

        display_menu();

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("\nInvalid input.\n");
            sleep(2);
            continue;
        }

        switch (choice) {

            case 1:
                clear_screen();

                printf("===============================================\n");
                printf("              RUNNING PROCESSES\n");
                printf("===============================================\n");

                display_processes();

                printf("\nPress Enter to return to menu...");
                getchar();
                getchar();
                break;

            case 2:
                process_control();

                printf("\nPress Enter to return to menu...");
                getchar();
                getchar();
                break;

            case 3:
                printf("\nExiting Multi-Process Task Manager...\n");
                return 0;

            default:
                printf("\nInvalid choice. Try again.\n");
                sleep(2);
        }
    }

    return 0;
}

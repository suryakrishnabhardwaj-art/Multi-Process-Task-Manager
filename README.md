# Multi-Process Task Manager for Linux

## 1. Project Overview

The Multi-Process Task Manager is a lightweight Linux system programming
application developed in C. It displays process information and allows
users to manage selected processes through a command-line interface.

## 2. Objectives

- Display processes running on a Linux system.
- Show process ID (PID), state, memory usage, and process name.
- Display detailed information for a selected process.
- Stop, continue, and terminate selected processes.
- Demonstrate Linux process management using system interfaces and signals.

## 3. Technologies Used

- Programming Language: C
- Operating System: Linux / Ubuntu
- Compiler: GCC
- Build Tool: GNU Make
- Process Information: Linux /proc filesystem
- Process Control: kill() and POSIX signals

## 4. Features

1. Display all visible processes.
2. View information about a selected PID.
3. Stop a process using SIGSTOP.
4. Continue a stopped process using SIGCONT.
5. Request process termination using SIGTERM.
6. Validate numeric menu and PID input.

Process information includes PID, process state, resident memory usage,
and process name.

## 5. Project Structure

    Multi-Process-Task-Manager/
    ├── task_manager.c
    ├── Makefile
    ├── README.md
    ├── .gitignore
    ├── docs/
    └── screenshots/

## 6. Requirements

- Linux or Ubuntu
- GCC compiler
- GNU Make

Install the required tools on Ubuntu if necessary:

    sudo apt update
    sudo apt install build-essential

## 7. Compilation

Open a terminal in the project directory and run:

    make clean
    make

Alternatively, compile directly:

    gcc -Wall -Wextra -O2 task_manager.c -o task_manager

## 8. Execution

Run the program:

    ./task_manager

The main menu provides these options:

1. Display All Processes
2. Control Selected Process
3. Exit

## 9. Process Control

After selecting a PID, the control menu provides:

1. View Process Information
2. Terminate Process (SIGTERM)
3. Stop Process (SIGSTOP)
4. Continue Process (SIGCONT)
5. Back to Main Menu

SIGTERM requests graceful termination. SIGSTOP stops a process, and
SIGCONT allows a stopped process to continue. Process control depends
on operating-system permissions.

## 10. Testing

The prototype was tested using a user-created sleep process.

Example test setup:

    sleep 300 &
    pgrep sleep

The program was used to view the process, stop it, continue it, and
request termination. The process was then checked using pgrep.

Additional tests included displaying the process list and starting
and exiting the application.

## 11. Safety Notes

Use this program only on processes you own or are authorized to manage.
For demonstrations, create a test process such as sleep 300. The program
rejects PID values less than or equal to 1 and its own PID, checks process
ownership, and requires confirmation before sending SIGTERM. Operating-system
permissions still apply; these checks do not guarantee complete protection.

## 12. Limitations and Future Enhancements

- The current interface is command-line based.
- The process list is not sorted or paginated.
- Ownership checks and termination confirmation are implemented.
- Future versions may improve permission diagnostics and add CPU usage,
  process sorting, search, and automatic refresh.

## 13. Conclusion

The project demonstrates Linux process inspection through /proc and
process management through POSIX signals. It provides a small prototype
for learning operating-system concepts and basic system programming.

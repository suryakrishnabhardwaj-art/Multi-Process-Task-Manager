# PROJECT REPORT
# Multi-Process Task Manager for Linux

## 1. Abstract

The Multi-Process Task Manager for Linux is a lightweight system
programming project implemented in C. It provides a command-line
interface for displaying process information and controlling a
selected process.

The application reads process information from the Linux /proc
filesystem. It displays process IDs, process states, resident memory
usage, and process names. It also provides options to view a selected
process, request termination using SIGTERM, stop it using SIGSTOP,
and resume it using SIGCONT.

The project demonstrates operating-system concepts including processes,
process states, Linux system interfaces, and signal-based process control.

## 2. Problem Statement

Linux systems can run many processes simultaneously. Users need a simple
way to inspect process information and perform basic process-management
operations from a terminal.

The objective is to develop a lightweight task manager that displays
process information and allows users to control selected processes.

## 3. Objectives

- Display processes accessible through the Linux /proc filesystem.
- Show each process's PID, state, memory usage, and name.
- Provide detailed information for a selected process.
- Support stopping, continuing, and requesting termination of a process.
- Validate user input and handle basic errors.
- Demonstrate process-management operations in a controlled environment.

## 4. Technologies Used

- Language: C
- Operating System: Linux / Ubuntu
- Compiler: GCC
- Build system: GNU Make
- Process information: /proc filesystem
- Process-control interface: kill()
- Signals: SIGTERM, SIGSTOP, SIGCONT
- Version control: Git and GitHub

## 5. System Design

The application follows a menu-driven design.

1. Main Menu
   - Display all processes.
   - Control a selected process.
   - Exit the application.

2. Process Information Module
   - Reads process names from /proc/<PID>/comm.
   - Reads process state and resident memory from
     /proc/<PID>/status.
   - Displays the collected information in a table.

3. Process Control Module
   - Checks whether the selected PID can be accessed.
   - Displays detailed information about the selected process.
   - Sends SIGTERM to request termination.
   - Sends SIGSTOP to stop the process.
   - Sends SIGCONT to resume the process.

4. Build Module
   - Uses the Makefile to compile the C source code with GCC.

## 6. Implementation Approach

The program scans numeric entries in the /proc directory to identify
process IDs. It reads the corresponding process files to collect the
process name, state, and resident memory information.

The menu accepts a process ID for process-control operations. The kill()
system interface is used to send signals to the selected process,
subject to operating-system permissions.

SIGTERM requests graceful termination. SIGSTOP suspends a process, while
SIGCONT allows a stopped process to continue. The program reports whether
the signal could be sent; successful signal delivery does not by itself
prove that termination has completed.

## 7. Testing and Validation

The prototype was tested in Ubuntu using a test process created by the
team.

Test setup:

    sleep 300 &
    pgrep sleep

The test process was selected through the application. The following
scenarios were checked:

| Test | Expected result | Observed result |
|------|-----------------|-----------------|
| Launch application | Main menu appears | Passed |
| Display processes | Process table appears | Passed |
| View selected test PID | Process details appear | Passed |
| Stop test process | State becomes stopped | Passed |
| Continue test process | Process resumes | Passed |
| Terminate test process | Termination is requested | Passed |
| Verify termination | Test process disappears from pgrep | Passed |
| Compile with Makefile | Program compiles | Passed after input-validation fix |

Testing was performed with a user-created sleep process rather than by
intentionally terminating critical system processes.

## 8. Results

The prototype successfully demonstrated process listing and basic
signal-based control of a selected test process. The application
compiled using the supplied Makefile without compiler warnings after
the input-validation correction.

## 9. Safety and Limitations

Process control depends on the user's permissions and the target process.
The implementation rejects PID values less than or equal to 1 and
the manager's own PID, checks that the selected process belongs to the
current user, and asks for confirmation before sending SIGTERM. These
checks reduce risk but are not a complete security boundary. Users must
only manage processes they own or are authorized to manage.

The application currently provides a terminal interface, lists visible
processes without sorting, and does not provide advanced CPU monitoring
or automatic refresh. Additional ownership checks and explicit
termination confirmation would improve safety before wider deployment.

## 10. Future Enhancements

- Improve permission diagnostics and handling of processes that exit
  while being inspected.
- Add sorting and filtering by PID, name, or memory usage.
- Add CPU usage and additional process statistics.
- Add automatic refresh and pagination.
- Improve error messages for permission failures and processes that exit
  while being inspected.

## 11. Conclusion

The Multi-Process Task Manager is a practical demonstration of Linux
process inspection and signal-based control using C. It connects
operating-system theory with implementation through the /proc filesystem,
system calls, and POSIX signals. The prototype provides a foundation for
further improvements in safety, usability, and monitoring capabilities.

## 12. Team Details

Project: Multi-Process Task Manager for Linux

| Team Member | Student ID |
|-------------|------------|
| Chaitanya | 2520090094 |
| Bhardwaj | 2520090182 |
| Salman | 2520089070 |

Faculty / Mentor: Mrs. Alekhya Dasari

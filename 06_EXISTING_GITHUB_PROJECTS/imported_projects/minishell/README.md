# Linux Minishell (msh) — Custom POSIX Command Interpreter

[![OS: Linux](https://img.shields.io/badge/OS-Linux%20POSIX-blue.svg)]()
[![Language: C11](https://img.shields.io/badge/Language-C11-orange.svg)]()
[![Status: Completed](https://img.shields.io/badge/Status-Completed-brightgreen.svg)](PROJECT_STATUS.md)
[![Whitepaper](https://img.shields.io/badge/Whitepaper-IEEE%20Format-purple.svg)](whitepaper/whitepaper.md)
[![Original Source](https://img.shields.io/badge/Original%20Repo-My__projects-lightgrey.svg)](https://github.com/DAravindReddy/My_projects)

A minimalist Unix command-line interpreter developed in C to demonstrate Linux operating system internals, process lifecycle management, POSIX signal handling, dynamic multi-stage inter-process communication (IPC) pipelines, and asynchronous job control.

---

## 1. System Features

- **Custom & Dynamic Prompt:** Default prompt `msh>`. Dynamically customizable via the environment variable `PS1=NEW_PROMPT`.
- **External Command Execution:** Creates child processes via `fork()`, executes binary images with `execvp()`, and synchronizes state via `waitpid()`.
- **Special Shell Variables:**
  - `echo $?`: Prints the exit status code of the last executed command.
  - `echo $$`: Prints the Process ID (PID) of the running `msh` shell instance.
  - `echo $SHELL`: Prints the executable path of `msh`.
- **Asynchronous Signal Handling:**
  - `Ctrl+C` (`SIGINT`): Terminates active foreground child processes without killing `msh`.
  - `Ctrl+Z` (`SIGTSTP`): Suspends foreground processes, displays child PID, and transitions them to the background job table.
  - `SIGCHLD`: Asynchronously reaps completed background children to eliminate zombie processes.
- **Built-in Shell Commands:**
  - `cd <dir>`: Changes the current working directory.
  - `pwd`: Prints the absolute working directory path.
  - `exit`: Cleanly terminates `msh`.
- **Background Execution & Job Control:**
  - Trailing ampersand (`&`) runs commands asynchronously (e.g., `sleep 50 &`).
  - `jobs`: Lists all running and stopped background processes.
  - `fg <pid>`: Brings a background or stopped process into the foreground.
  - `bg <pid>`: Resumes a stopped process in the background.
- **Dynamic Multi-Stage IPC Pipelines:**
  - Arbitrary dynamic pipeline chaining supporting any number of piped stages:
    ```bash
    msh> ls | wc
    msh> ls -l /dev | grep tty | wc -l
    ```

---

## 2. Dynamic Pipeline Architecture

```mermaid
graph LR
    P0["Child 0: ls -l /dev"] -->|pipe[0]| P1["Child 1: grep tty"]
    P1 -->|pipe[1]| P2["Child 2: wc -l"]
    P2 -->|stdout| Terminal["Terminal Output"]
```

---

## 3. Building and Running

### Linux / WSL / POSIX Environment:
```bash
# Build binary
make

# Run Shell
./msh
```

---

## 4. Documentation Index

- [System Architecture Specification](architecture/system_architecture.md)
- [Original Project Requirements & Description](documentation/project_description.txt)
- [Formal Research Whitepaper](whitepaper/whitepaper.md)
- [Project Status & Verification](PROJECT_STATUS.md)

---

## 5. Attribution
- **Author:** **Aravind Reddy** ([@DAravindReddy](https://github.com/DAravindReddy))
- **Original Repository:** [`https://github.com/DAravindReddy/My_projects`](https://github.com/DAravindReddy/My_projects)
- **License:** [MIT License](LICENSE)

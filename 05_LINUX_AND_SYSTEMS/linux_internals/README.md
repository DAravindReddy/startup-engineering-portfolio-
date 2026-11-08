# Linux Systems Programming & Kernel Internals (POSIX C)

[![Standard: POSIX.1-2008](https://img.shields.io/badge/Standard-POSIX.1--2008-blue.svg)](README.md)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)
[![Platform: Linux](https://img.shields.io/badge/Platform-Linux-orange.svg)](README.md)

A comprehensive systems engineering repository of low-level Linux operating system programming, POSIX system calls, Inter-Process Communication (IPC), multi-threading synchronization, and process lifecycle management.

---

## Architectural Domains

### 1. POSIX System Calls (`systemcalls/`)
- **Process Management:** `fork()`, `vfork()`, `execvp()`, `wait()`, `waitpid()`.
- **Process Memory & Descriptors:** File descriptor table duplication via `dup()`, `dup2()`, standard I/O redirection.
- **Low-Level I/O:** `open()`, `read()`, `write()`, `lseek()`, `close()`.
- **Filesystem Metadata:** `stat()`, `fstat()`, `lstat()`, inode permissions.

### 2. Inter-Process Communication (`ipc/`)
- **Anonymous Pipes (`pipe()`):** Unidirectional byte streams between parent and child processes.
- **Named Pipes (FIFOs `mkfifo()`):** Bidirectional filesystem-persistent IPC channels across unrelated processes.
- **Signals (`sigaction()`, `kill()`):** Asynchronous kernel signal delivery, signal masks, and signal handlers (`SIGINT`, `SIGCHLD`, `SIGUSR1`).
- **Shared Memory & Semaphores:** POSIX shared memory segments (`shm_open()`, `mmap()`) with mutex synchronization.

### 3. POSIX Threads & Concurrency (`threads/`)
- Multi-threading lifecycle: `pthread_create()`, `pthread_join()`, `pthread_detach()`.
- Race condition mitigation: `pthread_mutex_t` and `pthread_cond_t` condition variables.
- Thread-safe producer-consumer circular queues.

---

## Directory Structure

```text
linux_internals/
├── README.md                          # Master overview and architectural guide
├── LICENSE                            # MIT License
├── process.txt                        # Process lifecycle states (Task Struct, PCB)
├── categories_of-system_call.txt      # System call categorization
│
├── systemcalls/                       # fork, exec, dup, wait, file descriptors
├── ipc/                               # Pipes, FIFOs, Signals, Shared Memory
└── threads/                           # POSIX pthreads, mutexes, condition variables
```

---

## Author & Attribution
Authored by **Aravind Reddy** ([@DAravindReddy](https://github.com/DAravindReddy)).  
Standardized and curated for Kernelwise Labs portfolio monorepo.

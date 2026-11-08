# System Architecture: Linux Minishell (msh)

**Domain:** POSIX Linux System Programming  
**Author:** Aravind Reddy ([@DAravindReddy](https://github.com/DAravindReddy))  

---

## 1. Architectural Overview

**Minishell (`msh`)** is a custom POSIX command interpreter implemented in C. It serves as an operating systems research platform demonstrating process scheduling, POSIX signal handling, asynchronous child harvesting, and arbitrary n-stage inter-process communication (IPC) pipelines:

```mermaid
flowchart TD
    subgraph User_Boundary ["User Command Line Interface"]
        Input["User Input: 'ls -l /dev | grep tty | wc -l' or 'sleep 10 &'"]
        Prompt["Dynamic Shell Prompt (PS1 / 'msh>')"]
    end

    subgraph Tokenizer_and_Classifier ["Lexical Analysis & Command Dispatcher"]
        Parser["Command Lexer & Environment Expander ($?, $$, $SHELL)"]
        Classifier{"Check Command Type"}
    end

    subgraph Builtins_Handler ["Internal Commands Engine"]
        BuiltinExec["Internal Execution: cd / pwd / exit / jobs / fg / bg"]
    end

    subgraph Pipeline_Engine ["Dynamic IPC Pipeline Engine (n_pipe.c)"]
        PipeFork["Dynamic Multi-Pipe Forker (pipe(), dup2(), close())"]
        Pipe0["Child 0 (STDOUT -> pipe[0][1])"]
        Pipe1["Child 1 (STDIN <- pipe[0][0], STDOUT -> pipe[1][1])"]
        PipeN["Child N (STDIN <- pipe[N-1][0])"]
    end

    subgraph Job_Control ["Signal & Asynchronous Process Supervisor"]
        SigHandler["POSIX sigaction Traps (SIGINT, SIGTSTP, SIGCHLD)"]
        JobList["Background Job Table (PID, Command, State: RUNNING/STOPPED)"]
    end

    Prompt --> Input
    Input --> Parser
    Parser --> Classifier

    Classifier -- "Builtin" --> BuiltinExec
    Classifier -- "Piped Command" --> PipeFork
    Classifier -- "External Process" --> PipeFork

    PipeFork --> Pipe0
    PipeFork --> Pipe1
    PipeFork --> PipeN

    SigHandler <--> Job_Control
    Job_Control <--> PipeFork
    BuiltinExec <--> JobList
```

---

## 2. Dynamic Pipeline IPC Architecture (`n_pipe.c`)

When a user executes a chained command with $N$ pipes (e.g. $K = N + 1$ commands):
1. The shell dynamically allocates $N$ pipe file descriptor pairs: `int pipes[N][2]`.
2. For each command $i \in [0, K-1]$:
   - `fork()` creates a distinct child process.
   - If $i > 0$, `dup2(pipes[i-1][0], STDIN_FILENO)` binds the read end of the previous pipe to standard input.
   - If $i < K-1$, `dup2(pipes[i][1], STDOUT_FILENO)` binds the write end of the current pipe to standard output.
   - All unused pipe descriptors are deterministically closed across all forks to prevent deadlocks and broken pipe errors.
   - Child invokes `execvp()` to overlay the binary image.
3. The parent process closes all its pipe file descriptors and awaits child completion or registers them into the background job table if trailing `&` is detected.

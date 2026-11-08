# Implementing a Custom Linux Kernel System Call (Syscall)

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  
**Target Kernel:** Linux Kernel 5.x / 6.x (ARM & x86_64)  

---

## 1. Introduction

A system call is the primary mechanism through which user-space processes transition execution privilege from CPU User Mode (Ring 3 on x86, EL0 on ARM64) to CPU Supervisor/Kernel Mode (Ring 0 on x86, EL1 on ARM64).

This technical manual details the exact sequence for adding, registering, and compiling a custom syscall into the Linux kernel source tree.

---

## 2. Implementation Walkthrough

### Step 1: Create Syscall Implementation File
Create a new directory in the kernel tree `kernel/kernelwise/` and define `sys_kernelwise.c`:

```c
#include <linux/kernel.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(kernelwise_status, unsigned int, magic_code) {
    if (magic_code != 0xAA55) {
        pr_warn("[Kernelwise Syscall] Invalid authorization magic code: 0x%X\n", magic_code);
        return -EINVAL;
    }

    pr_info("[Kernelwise Syscall] Telemetry subsystem authorized (PID: %d)\n", current->pid);
    return 0xCAFE0001;
}
```
*Note:* `SYSCALL_DEFINE1` expands into standard architecture-specific wrapper code that prevents CVE-2009-0029 vulnerability (64-bit parameter sign-extension attacks).

### Step 2: Register Function Prototype
Append the prototype to `include/linux/syscalls.h`:
```c
asmlinkage long sys_kernelwise_status(unsigned int magic_code);
```

### Step 3: Register in the Syscall Table
- **For x86_64 (`arch/x86/entry/syscalls/syscall_64.tbl`):**
  ```text
  450   common   kernelwise_status   sys_kernelwise_status
  ```
- **For ARM64 / Universal Syscall Tables (`include/uapi/asm-generic/unistd.h`):**
  ```c
  #define __NR_kernelwise_status 450
  __SYSCALL(__NR_kernelwise_status, sys_kernelwise_status)
  ```

### Step 4: Add Directory to Kernel Makefile
In `kernel/kernelwise/Makefile`:
```makefile
obj-y := sys_kernelwise.o
```
In root `Makefile`:
```makefile
core-y += kernel/ mm/ fs/ ipc/ security/ crypto/ kernel/kernelwise/
```

### Step 5: Compile Kernel and Test
```bash
make -j$(nproc) bzImage modules
sudo make modules_install
sudo make install
```
Invoke using standard POSIX `syscall(__NR_kernelwise_status, 0xAA55)` in user space.

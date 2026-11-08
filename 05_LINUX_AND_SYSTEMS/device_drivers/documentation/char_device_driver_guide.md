# Linux Loadable Kernel Module (LKM) Character Device Driver Architecture

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  
**Standard:** Linux Kernel Subsystem Standard (VFS / cdev)  

---

## 1. Architectural Overview

Character device drivers represent the fundamental bridge between user-space POSIX system calls (`open`, `read`, `write`, `ioctl`, `close`) and kernel address spaces or physical hardware.

```
+-------------------------------------------------------------------------+
|                              USER SPACE                                 |
|   +-----------------------------------------------------------------+   |
|   |                  Application (test_user_app.c)                  |   |
|   |         open()       read()       write()       ioctl()         |   |
|   +-----------+------------+------------+--------------+------------+   |
+---------------|------------|------------|--------------|----------------+
|               | (SYSENTER / SVC / SYSCALL Trap)        |                |
+---------------v------------v------------v--------------v----------------+
|                             KERNEL SPACE                                |
|   +-----------------------------------------------------------------+   |
|   |                   Virtual File System (VFS)                     |   |
|   |                     struct file_operations                      |   |
|   +-----------+------------+------------+--------------+------------+   |
|               |            |            |              |                |
|               v            v            v              v                |
|         +-----------+ +-----------+ +-----------+ +-----------+         |
|         | dev_open  | | dev_read  | | dev_write | | dev_ioctl |         |
|         +-----------+ +-----------+ +-----------+ +-----------+         |
|               |            |            |              |                |
|               +------------+------+-----+--------------+                |
|                                   |                                     |
|                                   v                                     |
|                      +------------------------+                         |
|                      |  DEFINE_MUTEX(kw_mutex)|                         |
|                      |  Ring Buffer Storage   |                         |
|                      +------------------------+                         |
+-------------------------------------------------------------------------+
```

---

## 2. Dynamic Major/Minor Allocation & udev Automation

Traditional Linux drivers registered hardcoded major numbers, creating collisions. The modern Kernelwise driver pattern uses:

1. **Dynamic Number Allocation:**
   ```c
   alloc_chrdev_region(&dev_num, 0, 1, "kernelwise_char");
   ```
2. **Character Device Registration:**
   ```c
   cdev_init(&kw_cdev, &fops);
   cdev_add(&kw_cdev, dev_num, 1);
   ```
3. **Automatic udev Node Population:**
   ```c
   kw_class = class_create("kernelwise_class");
   kw_device = device_create(kw_class, NULL, dev_num, NULL, "kernelwise_char");
   ```
   This automatically instructs the `systemd-udevd` daemon to populate `/dev/kernelwise_char` with the assigned major and minor numbers without requiring manual `mknod`.

---

## 3. Kernel Memory Safety: copy_to_user & copy_from_user

In modern protected virtual memory architectures:
- Kernel code cannot directly dereference user-space pointers.
- Pointers passed by user space might be NULL, mapped to invalid memory, or unmapped pages that trigger page faults.
- Using `copy_to_user()` and `copy_from_user()` guarantees fault-tolerant copying: if a memory fault occurs during page translation, the kernel handles the exception and returns `-EFAULT` rather than initiating a kernel panic (Oops).

---

## 4. Concurrency Control with Kernel Mutexes

When multiple user threads access the same character device simultaneously:
- A non-reentrant device driver risks race conditions and data corruption.
- In `kernelwise_char`, `mutex_trylock(&kw_mutex)` guarantees single-instance exclusivity:
  ```c
  if (!mutex_trylock(&kw_mutex)) {
      return -EBUSY;
  }
  ```
- Any secondary process attempting concurrent access receives `EBUSY` immediately.

# Linux Kernel Scheduler & Interrupt Handling Architecture

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  

---

## 1. Linux Scheduling Classes

The Linux kernel implements a modular hierarchy of scheduling classes:

```
+--------------------------------------------------------------------------+
|                        SCHEDULER CLASS HIERARCHY                         |
+--------------------------------------------------------------------------+
| 1. Stop Class: Migrates CPUs, hotplug events (Highest Priority)          |
| 2. Deadline Class: Earliest Deadline First (SCHED_DEADLINE)              |
| 3. Real-Time Class: Fixed-priority preemptive (SCHED_FIFO, SCHED_RR)     |
| 4. Fair Class: Completely Fair Scheduler (CFS) / EEVDF (SCHED_NORMAL)    |
| 5. Idle Class: Runs CPU idle loop when runqueue is empty                 |
+--------------------------------------------------------------------------+
```

### Completely Fair Scheduler (CFS) & EEVDF
- Tracks per-task virtual runtime (`vruntime`).
- Runqueue is organized as a red-black tree indexed by `vruntime`.
- In Linux 6.6+, the Earliest Eligible Virtual Deadline First (EEVDF) algorithm replaces pure CFS, ensuring proportional latency bounds along with weighted throughput.

---

## 2. Interrupt Subsystem: Top Half vs. Bottom Half

Handling hardware interrupts requires balancing immediate hardware latency requirements against system preemption:

```
+------------------------------------------------------------------------+
| Hard IRQ (Top Half):                                                   |
| - Runs with local CPU interrupts disabled.                             |
| - Acknowledges peripheral hardware IRQ line.                           |
| - Enqueues data into kernel buffer.                                    |
| - Schedules bottom half and exits immediately (< 5 microseconds).      |
+-----------------------------------+------------------------------------+
                                    |
                                    v
+------------------------------------------------------------------------+
| Bottom Half (Deferred Execution):                                      |
| - Softirqs: Reentrant, high-throughput (Network RX/TX, RCU, Timers).    |
| - Tasklets: Built on softirqs, serialized per CPU.                     |
| - Workqueues: Executed in kernel thread context (`kworker`),           |
|   allowing sleepable operations, memory allocations, and mutex locks.  |
| - Threaded IRQs: Modern Linux standard (`request_threaded_irq`).       |
+------------------------------------------------------------------------+
```

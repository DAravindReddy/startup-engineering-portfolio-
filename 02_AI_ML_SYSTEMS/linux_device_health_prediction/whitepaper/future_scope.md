# Future Scope & Industrial Roadmap

## 1. Technological Roadmap

### Phase 1: eBPF Low-Level Kernel Probing
- Implement lightweight eBPF (extended Berkeley Packet Filter) programs attached to `tracepoint:irq:irq_handler_entry` and `kprobe:do_sys_open`.
- Enable kernel stack trace capture directly upon model anomaly trigger without userspace context-switch overhead.

### Phase 2: Online Incremental Learning
- Replace batch-trained Isolation Forest with Streaming Half-Space Trees (HS-Trees) or Incremental Isolation Forests to adapt to hardware aging, new peripheral attachments, and seasonal temperature variations without retraining from scratch.

### Phase 3: Autonomous Closed-Loop Remediation
- Interface the diagnostic engine with systemd D-Bus and Linux cgroup v2 APIs.
- Automatically throttle rogue memory-leaking processes or reassign IRQ affinities across CPU cores (`/proc/irq/<N>/smp_affinity`).

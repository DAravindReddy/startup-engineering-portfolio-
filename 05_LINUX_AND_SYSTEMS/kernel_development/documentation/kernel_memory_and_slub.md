# Linux Kernel Memory Management & SLUB Allocator

**Author:** Aravind Reddy  
**Organization:** Kernelwise Labs  

---

## 1. Physical Memory Zones & The Buddy Allocator

The Linux kernel segments physical memory into zones based on hardware addressing boundaries:
- `ZONE_DMA`: First 16 MB of RAM (legacy ISA device direct memory access).
- `ZONE_DMA32`: Physical memory range accessible by 32-bit DMA devices (up to 4 GB).
- `ZONE_NORMAL`: Directly mapped physical memory in 64-bit kernels.
- `ZONE_HIGHMEM`: Memory on 32-bit systems requiring dynamic temporary page mapping (above ~896 MB).

### Buddy Allocator
Physical memory is allocated in powers of two ($2^n$ contiguous $4\text{ KB}$ pages). 
- Orders range from $0$ ($4\text{ KB}$) to $10$ ($4\text{ MB}$).
- When an allocation of order $n$ cannot be satisfied, a larger block of order $n+1$ is split into two equal "buddies".
- Upon deallocation, adjacent free buddies are recombined to mitigate external fragmentation.

---

## 2. Object Caching: The SLUB Allocator

Because allocation of individual objects (e.g., `struct task_struct`, `struct inode`, `struct dentry`) via the buddy allocator would produce massive internal fragmentation, the kernel utilizes the SLUB allocator:

1. **`kmem_cache_create()`**: Allocates dedicated memory caches for recurring structs.
2. **`kmem_cache_alloc()` / `kmem_cache_free()`**: Fast O(1) pointer allocations from pre-initialized slabs.
3. **General Purpose `kmalloc()` / `kfree()`**: Allocates physically contiguous kernel memory backed by power-of-two general slab pools (`kmalloc-32`, `kmalloc-64`, `kmalloc-128`, ..., `kmalloc-8192`).
4. **`vmalloc()` / `vfree()`**: Allocates virtually contiguous but physically non-contiguous pages. Useful for large buffers where physical contiguity is unnecessary (e.g. loading kernel modules or large routing tables).

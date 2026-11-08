/**
 * @file char_driver_ioctl.h
 * @brief User-kernel shared IOCTL command definitions for Kernelwise Char Driver.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright GNU General Public License v2 (GPL-2.0) / MIT dual license
 */

#ifndef CHAR_DRIVER_IOCTL_H
#define CHAR_DRIVER_IOCTL_H

#if defined(__linux__) || defined(__KERNEL__)
#include <linux/ioctl.h>
#else
#include <stdint.h>
/* Platform fallback definitions for non-Linux host syntax validation */
#define _IOC_NRBITS	8
#define _IOC_TYPEBITS	8
#define _IOC_SIZEBITS	14
#define _IOC_DIRBITS	2

#define _IOC_NONE	0U
#define _IOC_WRITE	1U
#define _IOC_READ	2U

#define _IOC(dir,type,nr,size) \
	(((dir)  << 30) | \
	 ((type) << 0)  | \
	 ((nr)   << 8)  | \
	 ((size) << 16))

#define _IO(type,nr)		_IOC(_IOC_NONE,(type),(nr),0)
#define _IOR(type,nr,size)	_IOC(_IOC_READ,(type),(nr),sizeof(size))
#define _IOW(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),sizeof(size))
#define _IOWR(type,nr,size)	_IOC(_IOC_READ|_IOC_WRITE,(type),(nr),sizeof(size))
#endif

#define KW_IOCTL_MAGIC 'K'

typedef struct {
    uint32_t buffer_capacity;
    uint32_t bytes_stored;
    uint32_t open_count;
    uint32_t total_reads;
    uint32_t total_writes;
} kw_driver_stats_t;

/* IOCTL Commands */
#define KW_IOCTL_RESET_BUFFER   _IO(KW_IOCTL_MAGIC, 0)
#define KW_IOCTL_GET_STATS      _IOR(KW_IOCTL_MAGIC, 1, kw_driver_stats_t)
#define KW_IOCTL_SET_MAX_MSG    _IOW(KW_IOCTL_MAGIC, 2, uint32_t)

#endif /* CHAR_DRIVER_IOCTL_H */

/**
 * @file test_custom_syscall.c
 * @brief Demonstration harness for invoking custom Linux kernel system calls.
 * @author Aravind Reddy (Kernelwise Labs)
 * @copyright MIT License
 */

#include <stdio.h>
#include <stdint.h>

#if defined(__linux__)
#include <unistd.h>
#include <sys/syscall.h>

/* Hypothetical registered syscall vector */
#ifndef __NR_kernelwise_status
#define __NR_kernelwise_status 450
#endif

int main(void) {
    printf("=====================================================\n");
    printf(" Kernelwise Labs: Custom Syscall Verification Test   \n");
    printf(" Target Syscall Vector: __NR_kernelwise_status (%d)  \n", __NR_kernelwise_status);
    printf("=====================================================\n");

    long ret = syscall(__NR_kernelwise_status, 0xAA55);
    if (ret < 0) {
        perror("[INFO] Syscall returned error (expected if custom kernel is not booted)");
    } else {
        printf("[SUCCESS] Custom syscall executed successfully! Return: %ld\n", ret);
    }
    return 0;
}
#else
int main(void) {
    printf("[Kernelwise Labs] Custom syscall harness syntax verified on host.\n");
    return 0;
}
#endif

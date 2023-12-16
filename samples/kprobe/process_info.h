#ifndef PROCESS_INFO_H
#define PROCESS_INFO_H

#include <linux/module.h>
#include <linux/mm_types.h>
#include <linux/fs.h>
#include <linux/slab.h> // For kmalloc and kfree
#include <linux/sched.h> // For 'current' and other process-related structures

// struct to hold process information
struct process_info {
    int pid;
    char *comm;
    char *cmdline;
};

// function prototype
struct process_info get_process_info(void);

#endif // PROCESS_INFO_H

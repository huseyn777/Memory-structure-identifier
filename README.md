# Memory-structure-identifier

Project implemented in C, which goal was to create kernel modules in Linux for identifying systems memory structure (logical & physical addresses, page table etc.). 


step1:

Linux kernel module that gets and prints virtual memory usage and layout information for a given process

step2:

Prints the multi-level page table information (i.e., hierarchical page table consisting of tables at various levels) of a process (does it only for first 2 tables)

app:

Allocates and deallocates memory dynamically

step4:

Module will parse the tables of the page table of the process with the given pid and will find out the respective physical memory address and will print out the physical address into the log file using printk

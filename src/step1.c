#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/mm.h>

static int processid = 1;

module_param(processid, int, 0000);


static void print_mem(struct task_struct *task) 
{
    struct mm_struct *mm;
    struct vm_area_struct *vma;
    int count = 0;
    mm = task->mm;
    printk("\nThis mm_struct has %d vmas.\n", mm->map_count);

    struct vm_area_struct *stack_vma;

    for (vma = mm->mmap ; vma ; vma = vma->vm_next) 
    {
        printk("\nVMA number %d: \n", ++count);
        printk("Start = 0x%lx, End = 0x%lx, Size = 0x%lx\n", vma->vm_start, vma->vm_end, vma->vm_end - vma->vm_start);

	if(mm->start_stack >= vma->vm_start && mm->start_stack <= vma->vm_end)
		stack_vma = vma;
    }

    printk("\nCode  Segment start = 0x%lx, end = 0x%lx, size = 0x%lx\n",mm->start_code, mm->end_code, mm->end_code - mm->start_code);
    printk("Data  Segment start = 0x%lx, end = 0x%lx, size = 0x%lx\n",mm->start_data, mm->end_data, mm->end_data - mm->start_data);
    printk("Heap  Segment start = 0x%lx, end = 0x%lx, size = 0x%lx\n",mm->start_brk, mm->brk, mm->brk - mm->start_brk);
    printk("Stack Segment start = 0x%lx, end = 0x%lx, size = 0x%lx\n",stack_vma->vm_start, stack_vma->vm_end, stack_vma->vm_end - stack_vma->vm_start);
    printk("Main arguments start = 0x%lx, end = 0x%lx, size = 0x%lx\n",mm->arg_start, mm->arg_end, mm->arg_end - mm->arg_start);
    printk("Environment variables start = 0x%lx, end = 0x%lx, size = 0x%lx\n",mm->env_start, mm->env_end, mm->env_end - mm->env_start);
    printk("Number of frames used by the process = 0x%lx\n",get_mm_rss(mm));
    printk("Total virtual memory used by the process = 0x%lx\n",mm->total_vm);
}

int init_module(void)
{
    printk(KERN_INFO "Module start\n");

    struct task_struct *task;

    printk("\nProcess ID: %d\n", processid);

    for_each_process(task) {
        if (task->pid == processid) {
            printk("%s[%d]\n", task->comm, task->pid);
            print_mem(task);
        }
    }

    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Module end\n");
}


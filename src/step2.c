#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <linux/highmem.h>

#define PD_PAGESIZE 4096
#define RESULT_SPACE 8192

static int p_id = 1;

module_param(p_id, int, 0000);

static unsigned long convert(unsigned long va, int d) {
    unsigned long temp = va >> (63-d);
    va = va << d;
    va = va >> d;
    unsigned long p = 1;
    int i; 
    for (i = 0; i < d; i++)
     	p*=2;
    if (temp % 2 == 1) {
        unsigned long sub = p-1;
        sub = sub << (64-d);
        va = va + sub;
    } 
    return va;
}



static void dump_pmd(int index1, int index2, unsigned long va) 
{
    unsigned long pmd_entry;
    pmd_t *p;

    p = (pmd_t *) va;

    int i;
    for (i = 0; i < 512; i++) {
        pmd_entry = pmd_val(p[i]);

        if (pmd_entry % 2 == 1) {
            printk("PMD entry in use: (%d, %d, %d)\n", index1, index2, i);

        }
    }
}

static void dump_pud(int index, unsigned long va) 
{
    unsigned long pud_entry;
    unsigned long va2;

    pud_t *p;

    p = (pud_t *) va;

    int i;
    for (i = 0; i < 512; i++) {
        pud_entry = pud_val(p[i]);

        if (pud_entry % 2 == 1) {
	    printk("------->\n");
            printk("Valid PUD entry in use (Table #, Entery#) : (%d, %d)\n", index, i);
            printk("Valid PUD entry accessed: %lx\n", pud_entry);


            va2 = (unsigned long) pud_entry + PAGE_OFFSET;

            va2 = convert(va2, 16);

            printk("Valid PUD Virtual Address: %lx\n", va2);

             //dump_pmd(index, i, va2);

	    printk("******* Valid PUD entry end *********\n");
        }
    }
}

static void dump_pgd(struct task_struct *task) 
{
    unsigned long pgd_entry;
    unsigned long va;
    int i;

    for (i = 0; i < 512; i++) {
        pgd_entry = pgd_val(task->mm->pgd[i]);
        if (pgd_entry % 2 == 1) {
            printk("Valid PGD entry in use: %d\n", i);
            printk("Valid PGD entry accessed: %lx\n", pgd_entry);

            va = (unsigned long) pgd_entry + PAGE_OFFSET;

            va = convert(va, 16);

            printk("Valid PGD Virtual Address : %lx\n", va);

            dump_pud(i, va);
            printk("-------- Valid PGD entry end ----------\n");
        }
    }
}


int init_module(void)
{
    printk(KERN_INFO "Module start\n");


    struct task_struct *task;

    printk("\nProcess ID: %d\n", p_id);

    for_each_process(task) {
        if (task->pid == p_id) {
            printk("%s[%d]\n", task->comm, task->pid);
            dump_pgd(task);
        }
    }

    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "Module end\n");
}

#include <linux/moduleparam.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/mm.h>

static int processpid = 1;
unsigned long virtaddr;
module_param(virtaddr, long, 0000);
module_param(processpid, int, 0000);

static unsigned long vaddr2paddr(unsigned long vaddr, struct task_struct *task) {
    pgd_t *pgd;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    unsigned long paddr = 0;
    unsigned long page_addr = 0;
    unsigned long page_offset = 0;

    printk("Virtual Address = 0x%lx\n", vaddr);
    pgd = pgd_offset(task->mm, vaddr);

    printk("pgd_val = 0x%lx\n", pgd_val(*pgd));
    if (pgd_none(*pgd)) {
        printk("Error: Entry not found in PGD table.\n");
        return -1;
    }

    pud = pud_offset(pgd, vaddr);
    printk("pud_val = 0x%lx\n", pud_val(*pud));
    if (pud_none(*pud)) {
        printk("Error: Entry not found in PUD table.\n");
        return -1;
    }

    pmd = pmd_offset(pud, vaddr);
    printk("pmd_val = 0x%lx\n", pmd_val(*pmd));
    if (pmd_none(*pmd)) {
        printk("Error: Entry not found in PMD table.\n");
        return -1;
    }

    pte = pte_offset_kernel(pmd, vaddr);
    printk("pte_val = 0x%lx\n", pte_val(*pte));
    if (pte_none(*pte)) {
        printk("Error: Entry not found in PTE table.\n");
        return -1;
    }

    page_addr = pte_val(*pte) & PAGE_MASK;
    page_offset = vaddr & ~PAGE_MASK;
    paddr = page_addr | page_offset;
    printk("Page Address = %lx, Page Offset = %lx\n", page_addr, page_offset);
    printk("Virtual Address = %lx\nPhysical Address = %lx\n", vaddr, paddr);

    return paddr;
}

int init_module(void)
{
	printk(KERN_INFO "INIT: Virtual to Physical address converter module.\n");

    struct task_struct *task;

    for_each_process(task) {
        if (task->pid == processpid) {
            printk("%s[%d]\n", task->comm, task->pid);
            vaddr2paddr(virtaddr, task);
            break;
        }
    }

	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "EXIT: Virtual to Physical address converter module.\n");
}

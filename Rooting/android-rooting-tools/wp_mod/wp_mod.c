#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/version.h>
#include <linux/kallsyms.h>
#include <asm/mmu_writeable.h>

#define DRIVER_AUTHOR "flar2 / MohammadAG"
#define DRIVER_DESCRIPTION "Defeat system write protect"
#define DRIVER_VERSION "0.1"

#define HIJACK_SIZE 12

static int sony_ric_enabled(void)
{
	return 0;
}

inline void arm_write_hook ( void *target, char *code )
{
    unsigned long *target_arm = (unsigned long *)target;
    unsigned long *code_arm = (unsigned long *)code;

    mem_text_write_kernel_word(target_arm, *code_arm);
    mem_text_write_kernel_word(target_arm + 1, *(code_arm + 1));
    mem_text_write_kernel_word(target_arm + 2, *(code_arm + 2));
}

void hijack_start ( void *target, void *new )
{
    unsigned char n_code[HIJACK_SIZE];

    //ldr pc, [pc, #0]; .long addr; .long addr
    memcpy(n_code, "\x00\xf0\x9f\xe5\x00\x00\x00\x00\x00\x00\x00\x00", HIJACK_SIZE);
    *(unsigned long *)&n_code[4] = (unsigned long)new;
    *(unsigned long *)&n_code[8] = (unsigned long)new;

    pr_info("Hooking function 0x%p with 0x%p\n", target, new);

    arm_write_hook(target, n_code);
}

static int __init wp_mod_init(void)
{
	pr_info("wp_xperia_mod: %s version %s\n", DRIVER_DESCRIPTION,
		DRIVER_VERSION);
	pr_info("wp_xperia_mod: by %s\n", DRIVER_AUTHOR);

	hijack_start((void *)kallsyms_lookup_name("sony_ric_enabled"), &sony_ric_enabled);

	return 0;
}

module_init(wp_mod_init)

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_VERSION(DRIVER_VERSION);
MODULE_LICENSE("GPL");

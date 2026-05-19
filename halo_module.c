#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init halo_init(void) {
    printk(KERN_INFO "Module Memuat Halo Billahi Robby !\n");
    printk(KERN_INFO "Module sekarang sedang berjalan.\n");
    return 0;
}

static void __exit halo_exit(void) {
    printk(KERN_INFO "Module sudah berhenti. jirlah!\n");
}

module_init(halo_init);
module_exit(halo_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BILLAHI");
MODULE_DESCRIPTION("Module pertama saya jir di linux kernenn !");
MODULE_VERSION("1.0");
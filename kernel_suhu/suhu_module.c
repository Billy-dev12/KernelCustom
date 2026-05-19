#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

static char *data = NULL;
module_param(data, charp, 0000);
MODULE_PARM_DESC(data, "Data buat debug");

static int __init suhu_init(void)
{
    struct file *f;
    char buf[64];
    loff_t pos = 0;
    int temp;
    
    printk(KERN_INFO "Nakula: Module suhu dimuat.\n");
    
    // Coba baca thermal zone yang ada
    printk(KERN_INFO "Nakula: Mencoba baca suhu...\n");
    
    // Cara 1: Coba baca dari file /sys
    f = filp_open("/sys/class/thermal/thermal_zone1/temp", O_RDONLY, 0);
    if (IS_ERR(f)) {
        printk(KERN_ERR "Nakula: Gagal buka file! Error: %ld\n", PTR_ERR(f));
        return PTR_ERR(f);
    }
    
    memset(buf, 0, sizeof(buf));
    kernel_read(f, buf, sizeof(buf) - 1, &pos);
    filp_close(f, NULL);
    
    // Konversi string ke integer
    if (kstrtoint(buf, 10, &temp) == 0) {
        printk(KERN_INFO "=========================================\n");
        printk(KERN_INFO "NAKULA_TEMP:%d.%d_END\n", temp / 1000, (temp % 1000) / 100);
        printk(KERN_INFO "=========================================\n");
    } else {
        printk(KERN_ERR "Nakula: Gagal parse suhu. Raw data: %s\n", buf);
    }
    
    return 0;
}

static void __exit suhu_exit(void)
{
    printk(KERN_INFO "Nakula: Module suhu dicabut.\n");
}

module_init(suhu_init);
module_exit(suhu_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Nakula");
MODULE_DESCRIPTION("Baca suhu dari sysfs dalam kernel");
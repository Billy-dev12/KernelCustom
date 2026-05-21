#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/file.h>

#define PROC_NAME "jawa_suhu"
static struct proc_dir_entry *proc_file;

// Fungsi tangguh untuk membaca teks langsung dari file sysfs internal kernel
static int baca_suhu_asli(void) {
    struct file *file;
    char buf[16];
    int temp = 0;
    loff_t pos = 0;
    ssize_t bytes_read;

    // Membuka file thermal_zone0 bawaan sistem secara langsung
    file = filp_open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY, 0);
    if (IS_ERR(file)) {
        // Jika zone0 gagal, coba fallback ke zone1
        file = filp_open("/sys/class/thermal/thermal_zone1/temp", O_RDONLY, 0);
        if (IS_ERR(file)) {
            return -1; // Gagal total jika dua-duanya tidak bisa dibuka
        }
    }

    memset(buf, 0, sizeof(buf));
    bytes_read = kernel_read(file, buf, sizeof(buf) - 1, &pos);
    filp_close(file, NULL);

    if (bytes_read > 0) {
        // Mengubah string angka dari sysfs menjadi integer murni
        if (kstrtoint(buf, 10, &temp) == 0) {
            return temp;
        }
    }
    return -1;
}

static int suhu_show(struct seq_file *m, void *v) {
    int temp = baca_suhu_asli();

    seq_printf(m, "=========================================\n");
    seq_printf(m, "  Suhu CPU (Nakula Monitor)\n");
    seq_printf(m, "  Sensor : ACPI Thermal Zone\n");
    
    if (temp < 0) {
        // Fallback jika permission membaca file internal diblokir sistem keamanan tingkat tinggi
        seq_printf(m, "  Suhu   : [Gagal Membaca Sensor Asli]\n");
        seq_printf(m, "  Status : Mode Simulasi diaktifkan\n");
        seq_printf(m, "  Suhu   : 45.2 °C (Simulasi)\n");
    } else {
        seq_printf(m, "  Suhu   : %d.%d °C\n", temp / 1000, (temp % 1000) / 100);
    }
    
    seq_printf(m, "=========================================\n");
    seq_printf(m, "\nTip: Jalankan 'watch -n 1 cat /proc/jawa_suhu'\n");
    seq_printf(m, "     untuk monitoring real-time!\n");
    
    return 0;
}

static int suhu_open(struct inode *inode, struct file *file) {
    return single_open(file, suhu_show, NULL);
}

static const struct proc_ops suhu_ops = {
    .proc_open = suhu_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init suhu_init(void) {
    printk(KERN_INFO "BIllahi : Module suhu dimulai!\n");
    proc_file = proc_create(PROC_NAME, 0, NULL, &suhu_ops);
    if (!proc_file) {
        return -ENOMEM;
    }
    printk(KERN_INFO "Nakula: /proc/%s berhasil dibuat!\n", PROC_NAME);
    return 0;
}

static void __exit suhu_exit(void) {
    proc_remove(proc_file);
    printk(KERN_INFO "BIllahi : Module suhu berhenti. jirlah!\n");
}

module_init(suhu_init);
module_exit(suhu_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BILLAHI");
MODULE_DESCRIPTION("Module suhu aman untuk kernel modern!");
MODULE_VERSION("1.1");

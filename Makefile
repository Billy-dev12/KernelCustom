# Tentukan subfolder tempat file source berada
SRC_DIR := kernel_suhu

# Beritahu kbuild untuk mencari objek di dalam subfolder
obj-m += $(SRC_DIR)/suhu_proc.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

KERNEL_BUILD := < kernel source directory >
KERNEL_CROSS_COMPILE := < your toolchain >

obj-m += wp_mod.o

all:
	make -C $(KERNEL_BUILD) CROSS_COMPILE=$(KERNEL_CROSS_COMPILE) M=$(PWD) modules

clean:
	make -C $(KERNEL_BUILD) M=$(PWD) clean 2> /dev/null
	rm -f modules.order *~

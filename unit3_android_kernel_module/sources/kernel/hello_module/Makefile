obj-m += hello.o
PWD := $(shell pwd)
CLANG := /home/asus/kernel/prebuilts/clang/host/linux-x86/clang-r450784e/bin/clang
KERNEL := /home/asus/kernel/out/common/
LD = /home/asus/kernel/prebuilts/clang/host/linux-x86/clang-r450784e/bin/ld.lld
all:
	make LD=$(LD) ARCH=arm64 CC=$(CLANG) -C $(KERNEL) M=$(PWD) modules

clean:
	make -C $(KERNEL) M=$(PWD) clean

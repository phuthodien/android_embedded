# KERNEL MODULE RPI4
***
## 1.BUILD IN DRIVER HELLO 
```
cd kernel/common/drivers
mkdir -p hello
```

## IMPLEMENT MAKEFILE
```
code hello/Makefile
```

### Makefile
```
obj-$(CONFIG_HELLO) += hello.o
```

## IMPLEMENT KCONFIG
```
code hello/Kconfig
```
### Kconfig
```
# Device Hello configuration
# By BHien 
menuconfig HELLO
    tristate "Hello String"
    default y
    help
      Say Y to include this module
      Say N will not build this module
      Say M to build this module but not include to kernel yet
```

## ADD MAKFILE AND KCONFIG TO DRIVER
```
code Makefile 
code Kconfig
```
### /drivers/Makefile
```
obj-$(CONFIG_HELLO) += hello/
```
### /drivers/Kconfig
```
source "drivers/hello/Kconfig"
```

## IMPLEMENT SOURCE
```
code hello/hello.c
```
### /kernel/common/driver/hello/hello.c
```
#include <linux/module.h>	 /* Needed by all modules */
#include <linux/kernel.h>	 /* Needed for KERN_INFO */
#include <linux/init.h>	 /* Needed for the macros */

///< The license type -- this affects runtime behavior
MODULE_LICENSE("GPL");

///< The author -- visible when you use modinfo
MODULE_AUTHOR("Bui Dinh Hien");

///< The description -- see modinfo
MODULE_DESCRIPTION("A simple Hello world LKM!");

static int __init hello_start(void)
{
	printk(KERN_INFO "Loading hello module...\n");
	printk(KERN_INFO "Hello world\n");
	return 0;
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Goodbye Mr.\n");
}

module_init(hello_start);
module_exit(hello_end);
```

## BUILD WITH Config.sh
```
cd ../../
```
### :~/kernel$ 
```
BUILD_CONFIG=common/build.config.rpi4 \
LTO=none \
FAST_BUILD=1 \
SKIP_MRPROPER=1 \
SKIP_DEFCONFIG=1 \
build/config.sh
```
***
## 2. BUILD LOADABLE KERNEL MODULE
```
code common/build.config.rpi4
```
### ADD MODULES TO MAKE_GOALS
```
MAKE_GOALS="....
....
modules
"
```
### CREATE FOLDER HELLO
```
cd && mkdir -p hello
cd hello
```

### IMPLEMENT SOURCE 
```
code hello.c
```
### IMPLEMENT MAKEFILE
```
obj-m += hello.o
PWD := $(shell pwd)
CLANG := /home/asus/kernel/prebuilts/clang/host/linux-x86/clang-r450784e/bin/clang
KERNEL := /home/asus/kernel/out/common/
LD = /home/asus/kernel/prebuilts/clang/host/linux-x86/clang-r450784e/bin/ld.lld
all:
	make LD=$(LD) ARCH=arm64 CC=$(CLANG) -C $(KERNEL) M=$(PWD) modules

clean:
	make -C $(KERNEL) M=$(PWD) clean

```
### MAKE
```
make
```

## 3. ADB TEST MODULE
### ADB CONNECT 
```
adb root
adb devices
adb push hello.ko /dev  
```

### INSMOD MODULE

```
insmod hello.ko
dmesg | tail
rmmod hello.ko
```


## 4.BUILD KERNEL WITH BAZEL 
### CONFIGURATION KERNEL RPI4 
```
cd kernel/
code common/BUILD.bazel
code common/build.config.rpi4
```
### common/build.config.rpi4
```
MAKE_GOALS="${MAKE_GOALS}
dtbs
modules
Image.gz
"

FILES="${FILES}
arch/arm64/boot/Image.gz
arch/arm64/boot/dts/broadcom/bcm2711-rpi-4-b.dtb
arch/arm64/boot/dts/broadcom/bcm2711-rpi-400.dtb
arch/arm64/boot/dts/broadcom/bcm2711-rpi-cm4.dtb
arch/arm64/boot/dts/broadcom/bcm2711-rpi-cm4s.dtb
"
```
### common/BUILD.bazel
```
load("//build/kernel/kleaf:common_kernels.bzl", "define_common_kernels","define_rpi4")

...
# Sync with build.config.rpi4
define_rpi4(
    name = "rpi4",
    outs = aarch64_gz_outs + [
        "arch/arm64/boot/dts/broadcom/bcm2711-rpi-4-b.dtb",
        "arch/arm64/boot/dts/broadcom/bcm2711-rpi-400.dtb",
        "arch/arm64/boot/dts/broadcom/bcm2711-rpi-cm4.dtb",
        "arch/arm64/boot/dts/broadcom/bcm2711-rpi-cm4s.dtb",
    ],
    module_outs = [
    ],
)
```
### Add function define_rpi to common_kernels.bzl
```
code build/kleaf/common_kernels.bzl
```
### kernel/build/kleaf/common_kernels.bzl
```
def define_rpi4(
        name,
        outs,
        build_config = None,
        module_outs = None,
        kmi_symbol_list = None,
        dist_dir = None):
    """Define target for rpi4.

    Note: This does not use mixed builds.

    Args:
        name: name of target. Usually `"rpi4"`.
        build_config: See [kernel_build.build_config](#kernel_build-build_config). If `None`,
          default to `"build.config.rpi4"`.
        outs: See [kernel_build.outs](#kernel_build-outs).
        module_outs: See [kernel_build.module_outs](#kernel_build-module_outs). The list of
          in-tree kernel modules.
        kmi_symbol_list: See [kernel_build.kmi_symbol_list](#kernel_build-kmi_symbol_list).
        dist_dir: Argument to `copy_to_dist_dir`. If `None`, default is `"out/{BRANCH}/dist"`.
    """

    if build_config == None:
        build_config = "build.config.rpi4"

    if dist_dir == None:
        dist_dir = "out/{branch}/dist".format(branch = BRANCH)

    kernel_build(
        name = name,
        outs = outs,
        # List of in-tree kernel modules.
        module_outs = module_outs,
        build_config = build_config,
        kmi_symbol_list = kmi_symbol_list,
    )

    kernel_modules_install(
        name = name + "_modules_install",
        kernel_build = name,
        # List of external modules.
        kernel_modules = [],
    )

    kernel_images(
        name = name + "_images",
        build_initramfs = True,
        kernel_build = name,
        kernel_modules_install = name + "_modules_install",
    )

    copy_to_dist_dir(
        name = name + "_dist",
        data = [
            name,
            name + "_images",
            name + "_modules_install",
        ],
        dist_dir = dist_dir,
        flat = True,
        log = "info",
    )

```
### :~/kernel$
### CHECK RPI4 HAS BEEN ADDED
```
tools/bazel query "kind('py_binary', //common:*)"
```
### BUILD KERNEL WITH BAZEL 
```
tools/bazel run --config=fast //common:rpi4_dist
```
### IMAGE WAS CREATED AT 

```
cd out/android13-5.15/dist && ls
```


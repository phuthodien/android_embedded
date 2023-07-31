# MAKE ANDROID 13
### INSTALL TOOL FOR UBUNTU 20.04

```
sudo apt-get install git-core gnupg flex bison build-essential zip curl zlib1g-dev libc6-dev-i386 libncurses5 lib32ncurses5-dev x11proto-core-dev libx11-dev lib32z1-dev libgl1-mesa-dev libxml2-utils xsltproc unzip fontconfig
```
```
sudo apt-get install bc coreutils dosfstools e2fsprogs fdisk kpartx mtools ninja-build pkg-config python3-pip
```
```
sudo apt install python-is-python3
sudo pip3 install meson mako jinja2 ply pyyaml dataclasses
```



### INSTALL REPO TOOL FOR UBUNTU 20.04
```
sudo apt install repo
```
```
mkdir -p ~/.bin
curl https://storage.googleapis.com/git-repo-downloads/repo > ~/.bin/repo
chmod a+rx ~/.bin/repo
echo 'PATH="${HOME}/.bin:${PATH}"' >> ~/.bashrc
source ~/.bashrc
```

### GIT CONFIG NAME AND EMAIL 
```
git config --global user.email "abcxyz@gmail.com"
git config --global user.name "RealaxER"
```

### IF YOU NEED CONFIG HTTP GIT
```
ls -a 
code .gitconfig OR vim .gitconfig
```

### .gitconfig
```
[http]
	cookiefile = %USERPROFILE%\\.gitcookies
	postBuffer = 1737418240
	version = HTTP/1.1
[user]
	name = ................
	email = ................
[color]
	ui = auto  
```

### INIALIZATION REPO
```
cd 
mkdir -p aosp/source/
cd aosp/source/
```

### REPO INIT 
```
repo init -u https://android.googlesource.com/platform/manifest -b android-13.0.0_r61
```

### IMPLEMENT REPO
```
curl --create-dirs -L -o .repo/local_manifests/manifest_brcm_rpi4.xml -O -L https://raw.githubusercontent.com/raspberry-vanilla/android_local_manifest/android-13.0/manifest_brcm_rpi4.xml
```

### INSTALL SOURCE
```
repo sync
sudo apt-get update
```

### COMPILE
```
. build/envsetup.sh
lunch aosp_rpi4-userdebug
make bootimage systemimage vendorimage -j$(nproc)
```


### MAKE FLASH IMAGES
```
./rpi4-mkimg.sh
```
***

# MAKE KERNEL

### CREATE FOLDER KERNEL
```
cd ..
mkdir -p kernel/
cd kernel/
```


### INITALIZATION REPO KERNEL
```
repo init -u https://android.googlesource.com/kernel/manifest -b common-android13-5.15-lts
curl --create-dirs -L -o .repo/local_manifests/manifest_brcm_rpi4.xml -O -L https://raw.githubusercontent.com/raspberry-vanilla/android_kernel_manifest/android-13.0/manifest_brcm_rpi4.xml
```


### INSTALL SOURCE KERNEL
```
repo sync
```


### BUILD IMAGE KERNEL
```
BUILD_CONFIG=common/build.config.rpi4 build/build.sh
```









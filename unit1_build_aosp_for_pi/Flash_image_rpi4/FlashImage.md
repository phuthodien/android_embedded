# FLASH IMAGE FOR RPI4


## Install app IMAGER for flash image rpi4
```
cd ..
snap install rpi-imager
```

### Change file ownership
```
sudo chown user:root source/out/target/product/rpi4/RaspberryVanillaAOSP13.....
```

### FLASH IMAGE
```
1.click "choose os"
2.click "use custom"
3.choose imager in folder aosp/source/out/target/product/rpi4/RaspberryVanillaAOSP13-20230706-rpi4.img
4.choose ramidisk storage 
5.write flash image
6.remove ramdisk from computer then connect ramdisk to computer
7.copy replace Image from aosp/kernel/out/dist/ to /media/user/boot/ (ramdisk)
```



### Tiếng Việt
```
1.Sau khi cài xong thì mở app imager lên 
2.Chọn choose os
3.kéo xuống chọn use custom để mở .image trong folder folder aosp/source/out/target/product/rpi4/RaspberryVanillaAOSP13-20230706-rpi4.img
4.chọn choose storage để chọn thẻ nhớ
5.Chọn xong bấm write thôi là nó sẽ ghi vào thẻ nhớ 
6.Sau khi flash xong thì android có thể chạy được rồi nhưng cần có kernel nữa để lập trình
7.Tháo usb ra cắm lại 1 lần để nhận ,lúc này có 4 thư mục mình cần thay đổi file Image trong thư mục boot của ramdisk bằng Image trong aosp/kernel/out/dist/
```

## cp Image kernel
```
cp kernel/out/dist/Image /media/asus/boot/
sync
```


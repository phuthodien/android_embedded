# ADIL FOR HALs
## 1. IMPLEMENT AIDL 
### Create file aidl
```
cd aosp/source
mkdir -p hardware/interfaces/change/aidl/android/hardware/change/
nano hardware/interfaces/change/aidl/android/hardware/change/IChange.aidl
```

#### IChange.aidl
```
package android.hardware.change ;

@VintfStability
interface IChange {
    String getChars();
    void putChars(in String msg);
}
```

***
## 2. IMPLEMENT AIDL STABLE 

### Create aidl_interface.
```
nano hardware/interfaces/change/aidl/Android.bp
```

```
aidl_interface {
    name: "android.hardware.change",
    vendor: true,
    srcs: ["android/hardware/change/*.aidl"],
    stability: "vintf",
    owner: "BHien",
    backend: {
        cpp: {
            enabled: false,
        },
        java: {
            sdk_version: "module_current",
        },
    },
}
```

### Update api aidl 
```
m android.hardware.change-update-api
```
### Build aidl stable
```
mmm hardware/interfaces/change/
```
### Add aidl stable for vendor
```
nano build/make/target/product/base_vendor.mk
```
```
PRODUCT_PACKAGES += \
    android.hardware.change \
```
### Check libaries backend 
```
cd out/soong/.intermediates/hardware/interfaces/change/aidl
find .
cd -
```

***
## 3. IMPLEMENT HALs
### Write HALs 
```
cd hardware/interfaces/change/aidl
mkdir -p default
cd default 
nano Change.cpp
```
```
#define LOG_TAG "Change"

#include <utils/Log.h>
#include <iostream>
#include <fstream>
#include "Change.h"

namespace aidl {
namespace android {
namespace hardware {
namespace change {

//String getChars();
ndk::ScopedAStatus Change::getChars(std::string* _aidl_return) {
    std::ifstream change_file;
    change_file.open("/dev/change");
    if(change_file.good()) {
        std::string line;
        change_file >> line;
        ALOGD("Change service: %s", line.c_str());
        *_aidl_return =  line;
    } else {
        ALOGE("Can not open /dev/change");
        return ndk::ScopedAStatus::fromServiceSpecificError(-1);
    }
    return ndk::ScopedAStatus::ok();
}

//void putChars(in String msg);
ndk::ScopedAStatus Change::putChars(const std::string& in_msg) {
    std::ofstream change_file;
    change_file.open ("/dev/change");
    if(change_file.good()) {
        change_file << in_msg;
        ALOGD("Change service: %s", in_msg.c_str());
    } else {
        ALOGE("Can not open /dev/change");
        return ndk::ScopedAStatus::fromServiceSpecificError(-1);
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace change
}  // namespace hardware
}  // namespace android
}  // namespace aidl
```

### Create header file aidl
nano Change.h
```
#pragma once

#include <aidl/android/hardware/change/BnChange.h>

namespace aidl {
namespace android {
namespace hardware {
namespace change {

class Change: public BnChange{
    public:
        //String getChars();
        ndk::ScopedAStatus getChars(std::string* _aidl_return);
        //void putChars(in String msg);
        ndk::ScopedAStatus putChars(const std::string& in_msg);
};

}  // namespace change
}  // namespace hardware
}  // namespace android
}  // namespace aidl
```
***
## 4. Implement service HALs
### Write service file
```
nano service.cpp
```
```
#define LOG_TAG "Change"
/*Các thư viện của binder_manager*/
#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
// thư viện Change.h của HALs AIDL đã viết ở trên
#include "Change.h"

/*Do nó quá dài nên cần phải using lại cho ngắn*/
using aidl::android::hardware::change::Change;
using std::string_literals::operator""s;

void log_line(std::string msg) {
    std::cout << msg << std::endl;
    ALOGD("%s", msg.c_str());
}

void log_err(std::string msg) {
    std::cout << msg << std::endl;
    ALOGE("%s", msg.c_str());
}

int main() {
    // Cho phép vendor gọi vendor bằng vndbinder call 
    android::ProcessState::initWithDriver("/dev/vndbinder");

    /*Cấu hình thread*/
    ABinderProcess_setThreadPoolMaxThreadCount(0);
    ABinderProcess_startThreadPool();

    /*Truy cập vào default của aidl change*/
    std::shared_ptr<Change> change = ndk::SharedRefBase::make<Change>();
    const std::string name = Change::descriptor + "/default"s;

    /*Check xem có trỏ đến được không*/
    if (change != nullptr) {
        if(AServiceManager_addService(change->asBinder().get(), name.c_str()) != STATUS_OK) {
            log_err("Failed to register IChange service");
            return -1;
        }
    } else {
        log_err("Failed to get IChange instance");
        return -1;
    }

    log_line("IChange service starts to join service pool");
    ABinderProcess_joinThreadPool();

    return EXIT_FAILURE; 
}
```

## 5. Implement configuration services HALs
```
nano Android.bp
```
```
cc_binary {
    name: "android.hardware.change-service",
    vendor: true,
    relative_install_path: "hw",
    init_rc: ["android.hardware.change-service.rc"],
    vintf_fragments: ["android.hardware.change-service.xml"],

    srcs: [
        "Change.cpp",
        "service.cpp",
    ],

    cflags: [
        "-Wall",
        "-Werror",
    ],

    shared_libs: [
        "libbase",
        "liblog",
        "libhardware",
        "libbinder_ndk",
        "libbinder",
        "libutils",
        "android.hardware.change-V1-ndk",
    ],
}
```



### Implement rc init
```
nano android.hardware.change-service.rc
```
```
service android.hardware.change-service /vendor/bin/hw/android.hardware.change-service
        interface aidl android.hardware.change.IChange/default
        class hal
        user system
        group system
```


### Implement device manifest
```
nano android.hardware.change-service.xml
```
```
<manifest version="1.0" type="device">
    <hal format="aidl">
        <name>android.hardware.change</name>
        <version>1</version>
        <fqname>IChange/default</fqname>
    </hal>
</manifest>
```

### Add device manifest in the framework manifest
```
cd && cd aosp/source
```
```
nano hardware/interfaces/compatibility_matrices/compatibility_matrix.7.xml
```
```
nano hardware/interfaces/compatibility_matrices/compatibility_matrix.current.xml
```
compatibility_matrix.current.xml and compatibility_matrix.7.xml
```
<hal format="aidl" optional="true">
    <name>android.hardware.change</name>
    <version>1</version>
    <interface>
        <name>IChange</name>
        <instance>default</instance>
    </interface>
</hal>
```
## Add service hals for vendor to build
```
nano build/make/target/product/base_vendor.mk
```
```
PRODUCT_PACKAGES += \
    android.hardware.change \
    android.hardware.change-service \
```

## Add service hals for vendor
```
nano build/target/product/base_vendor.mk
```
```
PRODUCT_PACKAGES += \
    android.hardware.change-service
```
## Build Hals and Hals Service 
```
source build/envsetup.sh 
lunch aosp_rpi4-userdebug
m all -j$(nproc)
```



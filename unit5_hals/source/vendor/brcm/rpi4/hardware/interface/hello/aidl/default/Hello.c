#define LOG_TAG "Hello"

#include <utils/Log.h>
#include <iostream>
#include <fstream>
#include "Hello.h"

namespace aidl {
namespace android {
namespace hardware {
namespace hello {

//String getChars();
ndk::ScopedAStatus Hello::getChars(std::string* _aidl_return) {
    std::ifstream hello_file;
    hello_file.open("/dev/hello");
    if(hello_file.good()) {
        std::string line;
        hello_file >> line;
        ALOGD("Hello service: %s", line.c_str());
        *_aidl_return =  line;
    } else {
        ALOGE("Can not open /dev/hello");
        return ndk::ScopedAStatus::fromServiceSpecificError(-1);
    }
    return ndk::ScopedAStatus::ok();
}

//void send_string(in String msg);
ndk::ScopedAStatus Hello::send_string(const std::string& in_msg) {
    std::ofstream hello_file;
    hello_file.open ("/dev/hello");
    if(hello_file.good()) {
        hello_file << in_msg;
        ALOGD("Hello service: %s", in_msg.c_str());
    } else {
        ALOGE("Can not open /dev/hello");
        return ndk::ScopedAStatus::fromServiceSpecificError(-1);
    }
    return ndk::ScopedAStatus::ok();
}

}  // namespace hello
}  // namespace hardware
}  // namespace android
}  // namespace aidl
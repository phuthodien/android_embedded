#pragma once

#include <aidl/android/hardware/hello/BnHello.h>

namespace aidl {
namespace android {
namespace hardware {
namespace hello {

class Hello: public BnHello{
    public:
        //String getChars();
        ndk::ScopedAStatus getChars(std::string* _aidl_return);
        //void send_string(in String msg);
        ndk::ScopedAStatus send_string(const std::string& in_msg);
};

}  // namespace hello
}  // namespace hardware
}  // namespace android
}  // namespace aidl
# ADIL FOR HALs
## 1. IMPLEMENT AIDL 
### Create file aidl
```
cd aosp/source
mkdir -p hardware/interfaces/hello/aidl/android/hardware/hello/
nano hardware/interfaces/hello/aidl/android/hardware/hello/IHello.aidl
```

#### android/hardware/hello/IHello.aidl
```
package android.hardware.hello ;

interface IHello {
    void send_string(in String string_init);
}
```

***
## IMPLEMENT AIDL STABLE 

### Create aidl_interface.
```
nano hardware/interfaces/hello/aidl/Android.bp
```

```
aidl_interface {
    name: "android.hardware.hello",
    vendor: true,
    srcs: ["android/hardware/hello/*.aidl"],
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
m android.hardware.hello-update-api
```
### Build aidl stable
```
mmm hardware/interfaces/hello/
```
***
## 2. Implement AIDL for HALs

mkdir -p vendor/bcrm/rpi4/hardware/interfaces/hello/aidl/android/hardware/hello/
#### android/hardware/hello/IHello.aidl
```
package android.hardware.hello ;
@VintfStability 
interface IHello {
    void send_string(in String string_init);
}
```

***
## IMPLEMENT AIDL STABLE 

### Create aidl_interface.
```
nano vendor/bcrm/rpi4/hardware/interfaces/hello/aidl/Android.bp
```

```
aidl_interface {
    name: "android.hardware.hello",
    vendor: true,
    stability: "vintf",
    srcs: ["android/hardware/hello/*.aidl"],
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
m android.hardware.hello-update-api
```
### Build aidl stable
```
mmm vendor/brcm/rpi4/hardware/interfaces/hello/
```
***

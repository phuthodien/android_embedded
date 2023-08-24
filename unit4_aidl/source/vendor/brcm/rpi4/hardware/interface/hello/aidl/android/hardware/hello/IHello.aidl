package android.hardware.hello ;
@VintfStability 
interface IHello {
    void send_string(in String string_init);
}
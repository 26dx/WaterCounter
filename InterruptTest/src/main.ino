#include <Arduino.h>
#include <Rtc_Pcf8563.h>

Rtc_Pcf8563 rtc;
int flag = 0;

class pointerTest {
public:
  pointerTest(Rtc_Pcf8563& _rtc) {
        Serial.println(_rtc.getDay());
  }
};

void setup() {
        Serial.begin(9600);

        Serial.println("init");
        attachInterrupt(0, int0, RISING);
        attachInterrupt(1, int1, RISING);

        Serial.println(rtc.formatDate());

        pointerTest point(rtc);

}
void int0() {
        Serial.println("int0");
        flag = 1;
}
void int1() {
        Serial.println("int1");
}
void loop() {
 if (flag) {
   flag = 0;
   Serial.println(rtc.formatDate());
 }
}

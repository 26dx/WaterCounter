#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
        Serial.begin(9600);
        Serial.println("$ init;");
        lcd.init();
        lcd.backlight();
        lcd.clear();
}
void loop() {

}

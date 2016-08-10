#include <Arduino.h>

#include "CustomNumericMenuItem.h"
#include "MyRenderer.h"
#include "DataStore.h"
#include <EEPROM.h>
#include <MenuSystem.h>
#include <avr/sleep.h>

#define ADDRESS_0 0
#define ADDRESS_1 4

// counters
volatile uint8_t flagMenu, flagCounter, flagInMenu, buttonPressed = 0;
volatile uint16_t counterSleep = 0;

// dataStore
dataStore counterData01("Cold");
dataStore counterData02("Hot");

// forwart declaration
void on_item1_selected(MenuItem *p_menu_item);
void on_item2_selected(MenuItem *p_menu_item);
void on_item3_selected(MenuItem *p_menu_item);
void on_item4_selected(MenuItem *p_menu_item);
void on_item5_selected(MenuItem *p_menu_item);
void on_item6_selected(MenuItem *p_menu_item);
void on_item7_selected(MenuItem *p_menu_item);
void mi_return(MenuItem *p_menu_item);
const String format_int(const float value);

// Menu variables
MyRenderer my_renderer;
MenuSystem ms(my_renderer);

Menu mu1("1.Display   ");
MenuItem mu1_mi1("1.1.Overall ", &on_item1_selected);
MenuItem mu1_mi2("1.2.Monthly ", &on_item2_selected);
MenuItem mu1_mi3("1.3.Weekly  ", &on_item3_selected);
MenuItem mu1_mi4("1.4.Daily   ", &on_item7_selected);
Menu mu2("2.Setup     ");
NumericMenuItem mu2_mi1("2.1.In0 ", nullptr, 0, 0, 99999, 1, format_int);
NumericMenuItem mu2_mi2("2.2.In1 ", nullptr, 0, 0, 99999, 1, format_int);
MenuItem mu2_mi3("2.3.Reset   ", &on_item6_selected);
MenuItem mm_mi1("3.Exit      ", &mi_return);

// menu callback functions
const String format_int(const float value) {
        return String((long)value);
}
void on_item1_selected(MenuItem *p_menu_item) {
        Serial.println("Overall Sel");
        delay(1500); // so we can look the result on the LCD
}
void on_item2_selected(MenuItem *p_menu_item) {
        Serial.println("Monthly Sel");
        delay(1500); // so we can look the result on the LCD
}
void on_item3_selected(MenuItem *p_menu_item) {
        Serial.println("Weekly Sel");
        delay(1500); // so we can look the result on the LCD
}
void on_item6_selected(MenuItem *p_menu_item) {
        Serial.println("Reseting...");
        counterData01.set_value(0);
        counterData02.set_value(0);
        delay(1500); // so we can look the result on the LCD
}
void on_item7_selected(MenuItem *p_menu_item) {
}
void mi_return(MenuItem *p_menu_item) {
        flagInMenu = 0;
}

void setup() {
        Serial.begin(9600);

        // menu init
        ms.get_root_menu().add_menu(&mu1);
        mu1.add_item(&mu1_mi1);
        mu1.add_item(&mu1_mi2);
        mu1.add_item(&mu1_mi3);
        mu1.add_item(&mu1_mi4);
        ms.get_root_menu().add_menu(&mu2);
        mu2.add_item(&mu2_mi1);
        mu2.add_item(&mu2_mi2);
        mu2.add_item(&mu2_mi3);
        ms.get_root_menu().add_item(&mm_mi1);

        // init pins
        for (uint8_t input = 4; input <= 9; input++) {
                pinMode(input, INPUT);
                Serial.println(input);
        }
        pinMode(13, OUTPUT); // sleep pin
        digitalWrite(13, HIGH);

        // interrupts initialization
        attachInterrupt(0, interruptInput, RISING);
        attachInterrupt(1, interruptButton, RISING);

        // loading values from EEPROM
        counterData01.set_value(EEPROMReadLong(ADDRESS_0));
        counterData02.set_value(EEPROMReadLong(ADDRESS_1));

        // print current values
        valuesPrint();
}

void loop() {
        // put your main code here, to run repeatedly:
        if (flagCounter) {
                flagCounter = 0;
                valuesPrint();
        }
        if (flagMenu) {
                flagMenu = 0;
                menu();
        }
        /*  if (++counterSleep >= 1000) {
            counterSleep = 0;
            sleepEnable();
           }
           else
            delay(100);*/
}

void valuesPrint() {
        Serial.print("Input0: ");
        Serial.println(counterData01.get_value());
        Serial.print("Input1: ");
        Serial.println(counterData02.get_value());

        String textValue0="Cold";
        String textValue1="Hot";
        String textTime="2234";
        String textUnit="m3/m";
        String buffer;
        if (textValue0.length()<=5) {
                buffer+=textValue0;
                for (int i=0; i<=(5 - counterData01.get_description().length()); i++)
                        buffer+=" ";
        }
        if (textValue0.length()<=5) {
                buffer+=textValue0;
                for (int i=0; i<=(5 - counterData02.get_description().length()); i++)
                        buffer+=" ";
        }
        buffer+=textTime;
        Serial.println(buffer);

}

// three layers: 1st main menu, 2nd submenu and 3rd change parameters
void menu() {
        uint8_t incCounter = 0;
        mu2_mi1.set_value(counterData01.get_value());
        mu2_mi2.set_value(counterData02.get_value());
        flagInMenu = 1;
        ms.display();
        do {
                if (flagMenu) {
                        switch (buttonScan()) {
                        case 6:
                                ms.back();
                                ms.display();
                                break;
                        case 7:
                                ms.prev();
                                ms.display();
                                break;
                        case 8:
                                ms.next();
                                ms.display();
                                break;
                        case 9:
                                ms.select();
                                ms.display();
                                break;
                        default:
                                break;
                        }
                        flagMenu = 0;
                }
                if ((mu2_mi1.has_focus() || mu2_mi2.has_focus()) &&
                    (buttonScan() == 7 || buttonScan() == 8) && buttonPressed) {
                        //delay(300);
                        if (incCounter <= 40)
                                incCounter++;
                        if (incCounter <= 10)
                                ;
                        else if (incCounter <= 20) {
                                mu2_mi1.set_increment(10);
                                mu2_mi2.set_increment(10);
                        } else if (incCounter <= 30) {
                                mu2_mi1.set_increment(100);
                                mu2_mi2.set_increment(100);
                        } else if (incCounter <= 40) {
                                mu2_mi1.set_increment(1000);
                                mu2_mi2.set_increment(1000);
                        }
                        flagMenu = 1;
                } else {
                        buttonPressed = 0;
                        incCounter = 0;
                        mu2_mi1.set_increment(1);
                        mu2_mi2.set_increment(1);
                }
                delay(220);
        } while (flagInMenu);
        counterData01.set_value(mu2_mi1.get_value());
        counterData02.set_value(mu2_mi2.get_value());
        EEPROMWriteLong(ADDRESS_0, counterData01.get_value());
        EEPROMWriteLong(ADDRESS_1, counterData02.get_value());
        valuesPrint();
}

void interruptInput() {
        counterSleep = 0;
        if (digitalRead(4)) {
                counterData01.increment_value("test");
                EEPROMWriteLong(ADDRESS_0, counterData01.get_value());
        }
        else if (digitalRead(5)) {
                counterData02.increment_value("test");
                EEPROMWriteLong(ADDRESS_1, counterData02.get_value());
        }
        flagCounter = 1;
}

uint8_t buttonScan() {
        uint8_t in[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        for (uint8_t input = 6; input <= 9; input++) {
                in[input] = digitalRead(input);
                if (in[input])
                        return input;
        }
}

void interruptButton() {
        counterSleep = 0;
        flagMenu = 1;
        buttonPressed = 1;
}

void sleepEnable() {
        delay(100);
        digitalWrite(13, LOW);
        Serial.println("#sleep");
        sleep_enable();
        sleep_mode();
        sleep_disable();
        digitalWrite(13, HIGH);
}
void EEPROMWriteLong(int address, long value) {
        byte b0 = value;
        byte b1 = (value >> 8);
        byte b2 = (value >> 16);
        byte b3 = (value >> 24);
        EEPROM.write(address, b0);
        EEPROM.write(address + 1, b1);
        EEPROM.write(address + 2, b2);
        EEPROM.write(address + 3, b3);
}

long EEPROMReadLong(int address) {
        long b0 = EEPROM.read(address);
        long b1 = EEPROM.read(address + 1);
        long b2 = EEPROM.read(address + 2);
        long b3 = EEPROM.read(address + 3);
        return (b0 & 0xFF) + ((b1 << 8) & 0xFF00) + ((b2 << 16) & 0xFF0000) +
               ((b3 << 24) & 0xFF000000);
}

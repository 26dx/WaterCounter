#include "DataStore.h"

dataStore::dataStore(String _description) {
        Serial.println("Init dataStore");
        dataDesctiption = _description;
        day = 0;
        hour = 0;
        indexDay = 0;
        indexHour = 0;
        for (int i =0; i<24; dataValueHour[i++]=0);
        for (int i =0; i<31; dataValueDay[i++]=0);
}
void dataStore::set_value(long _dataValue) {
        dataValueOverall = _dataValue;
}
void dataStore::set_time_date(Rtc_Pcf8563& _rtc) {
        day = _rtc.getDay();
        hour = _rtc.getHour();
}
void dataStore::increment_value(Rtc_Pcf8563& _rtc) {
        dataValueOverall++;
        if (day!=_rtc.getDay()) {
                day = _rtc.getDay();
                if (indexDay<31)
                        indexDay++;
                else
                        indexDay = 0;
                dataValueDay[indexDay]++;
        } else
                dataValueDay[indexDay]++;
        if (hour!=_rtc.getHour()) {
                hour = _rtc.getHour();
                if (indexHour<24)
                        indexHour++;
                else
                        indexHour = 0;
                dataValueHour[indexHour]++;
        } else
                dataValueHour[indexHour]++;
}
void dataStore::reset() {
  for (int i =0; i<24; dataValueHour[i++]=0);
  for (int i =0; i<31; dataValueDay[i++]=0);
}
void dataStore::display_last12hours_to_lcd(LiquidCrystal_I2C& _lcd) {
        _lcd.clear();
        _lcd.setCursor(0, 0);
        _lcd.print("hot:");
        for (int i = 0; i < 8; i++) {
          _lcd.setCursor(5+i, 0);
          _lcd.write(i);
        }
}
long dataStore::get_value() {
        return dataValueOverall;
}
uint16_t dataStore::get_value_last_day() {
        return dataValueDay[indexDay];
}
uint16_t dataStore::get_value_last_hour() {
        return dataValueHour[indexHour];
}
// максимальное значение dataValueOverall 9 999 999 (7 разрядов)
// для выводу показаний достаточно 5 разрядов (99 999)
String dataStore::get_formated_value(long input, uint8_t divider) {
        String buffer = String(input/divider);
        return buffer;
}
String dataStore::get_description() {
        return dataDesctiption;
}
uint8_t dataStore::saveDataStore(uint8_t _startAddress) {
        uint8_t address = _startAddress;
        byte _byte[4] = {0,0,0,0};
        for (int i = 0; i<4; i++) {
                _byte[i] = dataValueOverall >> (8*i);
                EEPROM.write(address++, _byte[i]);
        }
        // запись данных по часам (24)
        for (int i = 0; i<24; i++) {
                for (int i2 = 0; i2<2; i2++) {
                        _byte[i2] = dataValueHour[i] >> (8*i2);
                        EEPROM.write(address++, _byte[i2]);
                }
        }
        EEPROM.write(address++, day);
        EEPROM.write(address++, indexDay);
        EEPROM.write(address++, hour);
        EEPROM.write(address++, indexHour);
        return address;
}
uint8_t dataStore::loadDataStore(uint8_t _startAddress) {
        uint8_t address = _startAddress;
        long _byte[4] = {0,0,0,0};
        for (int i=0; i<4; i++)
                _byte[i]=EEPROM.read(address++);
        dataValueOverall = (_byte[0] & 0xFF) + ((_byte[1] << 8) & 0xFF00) + ((_byte[2] << 16) & 0xFF0000) +
                           ((_byte[3] << 24) & 0xFF000000);
        for (int i = 0; i<24; i++) {
                for (int i2 = 0; i2<2; i2++) {
                        _byte[i2]=EEPROM.read(address++);
                }
                dataValueHour[i]=(_byte[0] & 0xFF) + ((_byte[1] << 8) & 0xFF00);
        }
        day = EEPROM.read(address++);
        indexDay = EEPROM.read(address++);
        hour = EEPROM.read(address++);
        indexHour = EEPROM.read(address++);
        return address;
}
void dataStore::printStorage24h() {
        for (int i = 0; i < 24; i++) {
                Serial.print("$ hour ");
                Serial.print(i);
                Serial.print(" ");
                Serial.println(dataValueHour[i]);
        }
        Serial.println("index hour:");
        Serial.println(indexHour);
}

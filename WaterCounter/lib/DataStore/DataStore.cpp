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
long dataStore::get_value() {
        return dataValueOverall;
}
uint8_t dataStore::get_value_day() {
        return dataValueDay[indexDay];
}
uint8_t dataStore::get_value_hour() {
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
        uint8_t _address = 0;
        byte _byte[4] = {0,0,0,0};
        for (int i = 0; i<4; i++) {
                _byte[i] = dataValueOverall >> (8*i);
                EEPROM.write(_startAddress + i, _byte[i]);
                _address = i;
        }
        return (++_address);
}
uint8_t dataStore::loadDataStore(uint8_t _startAddress) {
        long _byte[4] = {0,0,0,0};
        for (int i=0; i<4; i++)
                _byte[i]=EEPROM.read(_startAddress+i);
        dataValueOverall = (_byte[0] & 0xFF) + ((_byte[1] << 8) & 0xFF00) + ((_byte[2] << 16) & 0xFF0000) +
               ((_byte[3] << 24) & 0xFF000000);
        return (_startAddress+4);
}

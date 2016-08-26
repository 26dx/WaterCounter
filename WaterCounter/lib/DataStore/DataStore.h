/*
 *       TODO:
 *        1. Недельные показания
 *        2. Востановление часовых, дневных, недельных, месячных значений из EEPROM
 *        3. Восстановление данных из EEPROM во время создания объекта
 *        4. Запись данных в EEPROM в методе increment_value
 *        5. Исправить вывод временных показаний, в данный момен выводятся 0.01м3
 *        6. Добавить в вывод временных показаний единицы измерения показаний
 */

#include <Arduino.h>
#include <Rtc_Pcf8563.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

class dataStore {
public:
        dataStore(String _description);
        void set_value(long _dataValue);
        void set_time_date(Rtc_Pcf8563& _rtc);
        void increment_value(Rtc_Pcf8563& _rtc);
        void reset();
        void display_last12hours_to_lcd(LiquidCrystal_I2C& _lcd);
        long get_value();
        uint16_t get_value_last_day();
        uint16_t get_value_last_hour();
        String get_formated_value(long input, uint8_t divider);
        String get_description();
        uint8_t saveDataStore(uint8_t _startAddress);
        uint8_t loadDataStore(uint8_t _startAddress);
        void printStorage24h();
private:
        long dataValueOverall;
        uint16_t dataValueHour[24];
        uint16_t dataValueDay[31];
        String dataDesctiption;
        byte day;
        byte hour;
        byte indexDay;
        byte indexHour;
        uint8_t endAddress;
};

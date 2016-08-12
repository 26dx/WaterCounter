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

class dataStore {
public:
        dataStore(String _description);
        void set_value(long _dataValue);
        void increment_value(Rtc_Pcf8563 rtc);
        long get_value();
        String get_formated_value(long input);
        String get_description();
private:
        long dataValueOverall;
        uint8_t dataValueHour;
        uint8_t dataValueMonthly;
        uint8_t dataValueWeekly;
        uint8_t dataValueDaily;
        String dataDesctiption;
        byte currentHour;
        byte currentDay;
        byte currentWeek;
        byte lastWeek;
        byte weekday;
        byte currentMonth;
};

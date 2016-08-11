#include <Arduino.h>
#include "DataStore.h"

dataStore::dataStore(String _description) {
        dataDesctiption = _description;
}
void dataStore::set_value(long _dataValue) {
        dataValueOverall = _dataValue;
        dataValueDaily = 0;
        dataValueMonthly = 0;
        dataValueWeekly = 0;
        dataValueHour = 0;

}
// если date не совпадает с предыдущем значением,то сбрасываем соответственные счетчики - дневной, недельный, месячный
void dataStore::increment_value(Rtc_Pcf8563 rtc) {
        if (currentHour != rtc.getHour()) {
                currentHour = rtc.getHour();
                dataValueHour = 1;
        }
        else
                dataValueDaily++;
        if (currentDay != rtc.getDay()) {
                currentDay = rtc.getDay();
                dataValueDaily = 1;
        } else
                dataValueHour++;
        if (currentMonth != rtc.getMonth()) {
                currentMonth = rtc.getMonth();
                dataValueMonthly = 1;
        } else
                dataValueMonthly++;

        dataValueWeekly = 0;
        dataValueOverall++;
}
long dataStore::get_value() {
        return dataValueOverall;
}
// максимальное значение dataValueOverall 9 999 999 (7 разрядов)
// для выводу показаний достаточно 5 разрядов (99 999)
String dataStore::get_formated_value(long input) {
        String buffer = String(input/100);
        return buffer;
}
String dataStore::get_description() {
        return dataDesctiption;
}

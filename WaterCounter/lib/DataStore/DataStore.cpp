#include <Arduino.h>
#include "DataStore.h"

dataStore::dataStore(String _description, Rtc_Pcf8563 _startValues) {
        dataDesctiption = _description;
        day = _startValues.getDay();
        hour = _startValues.getHour();
        indexDay = 0;
        indexHour = 0;
        for (int i =0; i<24; dataValueHour[i++]=0);
        for (int i =0; i<30; dataValueDay[i++]=0);

}
void dataStore::set_value(long _dataValue) {
        dataValueOverall = _dataValue;
}
// если date не совпадает с предыдущем значением,то сбрасываем соответственные счетчики - дневной, недельный, месячный
void dataStore::increment_value(Rtc_Pcf8563 rtc) {
        dataValueOverall++;
        if (day!=rtc.getDay()) {
                day = rtc.getDay();
                if (indexDay<30)
                        indexDay++;
                else
                        indexDay = 0;
                dataValueDay[indexDay]++;
        }
        if (hour!=rtc.getHour()) {
                hour = rtc.getHour();
                if (indexHour<24)
                        indexHour++;
                else
                        indexHour = 0;
                dataValueHour[indexHour]++;
        }
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

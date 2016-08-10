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
}
// если date не совпадает с предыдущем значением,то сбрасываем соответственные счетчики - дневной, недельный, месячный
void dataStore::increment_value(String _date) {
        dataValueOverall++;
}
long dataStore::get_value() {
        return dataValueOverall;
}

String dataStore::get_description() {
        return dataDesctiption;
}

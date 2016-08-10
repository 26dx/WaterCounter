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
void dataStore::increment_value() {

}
long dataStore::get_value() {
        return dataValueOverall;
}

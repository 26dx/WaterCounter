#include <Arduino.h>

class dataStore {
public:
        dataStore(String _description);
        void set_value(long _dataValue);
        void increment_value(String _date);
        long get_value();
private:
        long dataValueOverall;
        uint8_t dataValueMonthly;
        uint8_t dataValueWeekly;
        uint8_t dataValueDaily;
        String dataDesctiption;
};

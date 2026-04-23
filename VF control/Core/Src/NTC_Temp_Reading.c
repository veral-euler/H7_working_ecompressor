#include "NTC_Temp_Reading.h"

float NTC_Read(uint16_t analog_val)
{
    double resistance = (analog_val * 10000.0f)/(65536.0f - analog_val);
    double temp_K = resistance/10000.0f;

    temp_K = log(temp_K);
    temp_K /= 4000.0f;
    temp_K += 1.0/(25.0f + 273.15f);
    temp_K = 1.0/temp_K;
    temp_K -= 273.15;

    return temp_K;
}

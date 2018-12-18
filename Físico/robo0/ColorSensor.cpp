#include "ColorSensor.hpp"

ColorSensor::ColorSensor(int name, int gain)
{
    switch (name)
    {
        case 1:
            sda_pin = L_COLOR_SENSOR_SDA;
            scl_pin = L_COLOR_SENSOR_SCL;
        break;
        case 2:
            sda_pin = R_COLOR_SENSOR_SDA;
            scl_pin = R_COLOR_SENSOR_SCL;
        break;
    }
    switch (gain)
    {
        case 1:
            sensor = new Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X, sda_pin, scl_pin);
        break;
        case 2:
            sensor = new Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X, sda_pin, scl_pin);
        break;
        case 3:
            sensor = new Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X, sda_pin, scl_pin);
        break;
        case 4:
            sensor = new Adafruit_TCS34725softi2c(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_60X, sda_pin, scl_pin);
        break;
    }
    if (!sensor->begin())
    {
        Serial.println("Sensor nao conectado");
    }
}

uint16_t* ColorSensor::getColors()
{
    sensor->getRawData(&colors[1], &colors[2], &colors[3], &colors[4]);
    return colors;
}

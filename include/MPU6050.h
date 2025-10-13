#ifndef MPU6050_DRIVER
#define MPU6050_DRIVER

#include <Wire.h>

class MPU6050{
    public:
        MPU6050(uint8_t address = 0x68);
        void begin();
        int16_t readAccelX();
        int16_t readAccelY();
        int16_t readAccelZ();
        int16_t readGyroX();
        int16_t readGyroY();
        int16_t readGyroZ();
        int16_t readTemp();
        float accelToG(int16_t raw);
        float rotToD(int16_t raw);
        float tempToC(int16_t raw);
    private:    
        uint8_t _addr;
        int16_t readRegister(uint8_t reg);
};

#endif
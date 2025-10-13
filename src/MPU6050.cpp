#include <MPU6050.h>

MPU6050::MPU6050(uint8_t address){
    _addr = address;
}

void MPU6050::begin(){
    Wire.beginTransmission(_addr);
    Wire.write(0x6B);
    Wire.write(0x00);
    Wire.endTransmission(true);
}

int16_t MPU6050::readRegister(uint8_t reg){
    Wire.beginTransmission(_addr);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(_addr, (uint8_t)2);

    while (Wire.available()<2);

    uint8_t high = Wire.read();
    uint8_t low = Wire.read();
    return (high<<8)|low;
}
// Accelerometer
int16_t MPU6050::readAccelX() { return readRegister(0x3B); }
int16_t MPU6050::readAccelY() { return readRegister(0x3D); }
int16_t MPU6050::readAccelZ() { return readRegister(0x3F); }

// Gyroscope
int16_t MPU6050::readGyroX() { return readRegister(0x43); }
int16_t MPU6050::readGyroY() { return readRegister(0x45); }
int16_t MPU6050::readGyroZ() { return readRegister(0x47); }

// Temperature
int16_t MPU6050::readTemp() { return readRegister(0x41); }

// Conversion functions
float MPU6050::accelToG(int16_t raw) { return raw / 16384.0; }
float MPU6050::rotToD(int16_t raw) { return (raw / 32768.0) * 250.0; }
float MPU6050::tempToC(int16_t raw) { return (raw / 340.0) + 36.53; }
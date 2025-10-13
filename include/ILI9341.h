#ifndef ILI9341_H
#define ILI9341_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

class ILI9341 {
public:
    ILI9341(uint8_t cs, uint8_t dc, uint8_t rst);
    void begin();
    void drawDashboardLabels();
    void showAccel(float ax, float ay, float az);
    void showTemp(float temp);
    void showGyro(float gx, float gy, float gz);
    void showStatus(bool ok = true);
    void clearReadings();

private:
    Adafruit_ILI9341 tft;
};

#endif

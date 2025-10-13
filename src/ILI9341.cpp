#include "ILI9341.h"

ILI9341::ILI9341(uint8_t cs, uint8_t dc, uint8_t rst) : tft(cs, dc, rst) {}

void ILI9341::begin() {
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(ILI9341_BLACK);
}

void ILI9341::drawDashboardLabels() {
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(5, 5);   tft.print("Accel X:");
    tft.setCursor(5, 30);  tft.print("Accel Y:");
    tft.setCursor(5, 55);  tft.print("Accel Z:");
    tft.setCursor(5, 80);  tft.print("Temp   :");
    tft.setCursor(5, 110); tft.print("Gyro X:");
    tft.setCursor(5, 135); tft.print("Gyro Y:");
    tft.setCursor(5, 160); tft.print("Gyro Z:");
    tft.setCursor(5, 190); tft.print("Status :");
}

void ILI9341::showAccel(float ax, float ay, float az) {
    tft.fillRect(120, 5, 80, 20, ILI9341_BLACK);
    tft.fillRect(120, 30, 80, 20, ILI9341_BLACK);
    tft.fillRect(120, 55, 80, 20, ILI9341_BLACK);
    tft.setCursor(120, 5);  tft.printf("%.2f g", ax);
    tft.setCursor(120, 30); tft.printf("%.2f g", ay);
    tft.setCursor(120, 55); tft.printf("%.2f g", az);
}

void ILI9341::showTemp(float temp) {
    tft.fillRect(120, 80, 80, 20, ILI9341_BLACK);
    tft.setCursor(120, 80); tft.printf("%.1f C", temp);
}

void ILI9341::showGyro(float gx, float gy, float gz) {
    tft.fillRect(120, 110, 80, 20, ILI9341_BLACK);
    tft.fillRect(120, 135, 80, 20, ILI9341_BLACK);
    tft.fillRect(120, 160, 80, 20, ILI9341_BLACK);
    tft.setCursor(120, 110); tft.printf("%.2f", gx);
    tft.setCursor(120, 135); tft.printf("%.2f", gy);
    tft.setCursor(120, 160); tft.printf("%.2f", gz);
}

void ILI9341::showStatus(bool ok) {
    tft.fillCircle(150, 200, 8, ok ? ILI9341_GREEN : ILI9341_RED);
}

void ILI9341::clearReadings() {
    tft.fillRect(120, 5, 80, 180, ILI9341_BLACK);
}

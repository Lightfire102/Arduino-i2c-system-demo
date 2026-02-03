////////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: Combined I2C demo.
- DS1307 RTC (RTClib) for timestamp
- Raw Wire.h reads from MPU-9250/MPU-6050 register block (Accel/Gyro/Temp)
AUTHOR: Justus Downing
NOTES FOR MPU-9250 / MPU-6500 BREAKOUTS (I2C MODE):
- Power with 3.3V just to be safe
- For I2C mode, NCS/CS MUST be tied HIGH (to VCC).
- Address is 0x69.
*/
////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES
////////////////////////////////////////////////////////////////////////////////////////
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Wire.h"     // I2C library
#include "RTClib.h"   // DS1307 RTC library
using namespace std;
////////////////////////////////////////////////////////////////////////////////////////
// DEFINES AND MACROS
////////////////////////////////////////////////////////////////////////////////////////
// MPU address: 0x68 if AD0 LOW, 0x69 if AD0 HIGH
const int MPU_I2C_SlaveAddress = 0x69;
// WHO_AM_I register (works for MPU-6050/6500/9250)
const uint8_t WHO_AM_I_REG = 0x75;
// Set to 1 to FORCE RTC time you need to change the timer (use once, then set back to 0)
#define FORCE_SET_RTC_TIME  0
////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] =
{
"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"
};
int16_t accel_x, accel_y, accel_z;
int16_t gyro_x, gyro_y, gyro_z;
int16_t temp;
////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////
void setup()
{
Serial.begin(115200);
Serial.write("\n\rRTC + MPU (RAW I2C) Demo\n\r");
// Start I2C
Wire.begin();
Wire.setClock(100000);
// -------------------------
// MPU: quick identity check
// -------------------------
Wire.beginTransmission(MPU_I2C_SlaveAddress);
Wire.write(WHO_AM_I_REG);
Wire.endTransmission(false);
Wire.requestFrom(MPU_I2C_SlaveAddress, 1, true);
uint8_t whoami = Wire.read();
Serial.print("WHO_AM_I = 0x");
Serial.println(whoami, HEX);
// Wake up MPU (PWR_MGMT_1 = 0x6B, write 0x00)
Wire.beginTransmission(MPU_I2C_SlaveAddress);
Wire.write(0x6B);
Wire.write(0x00);
Wire.endTransmission(true);
delay(50);
// -------------------------
// RTC init + set time
// -------------------------
if (!rtc.begin())
{
Serial.println("Couldn't find RTC");
Serial.flush();
while (1) delay(10);
}
// Set RTC to 2/1/2026 7:27 PM (19:27:00) once
if (!rtc.isrunning() || FORCE_SET_RTC_TIME)
{
Serial.println("Setting RTC time to 2026-02-01 19:27:00");
rtc.adjust(DateTime(2026, 2, 1, 19, 27, 0));
}
}
////////////////////////////////////////////////////////////////////////////////////////
void loop()
{
// -------------------------
// RTC timestamp (compact)
// -------------------------
DateTime now = rtc.now();
// Print: 2/1/26 (Sunday) 19:27:SS
Serial.print(now.month(), DEC);
Serial.print('/');
Serial.print(now.day(), DEC);
Serial.print('/');
Serial.print(now.year() % 100, DEC);
Serial.print(" (");
Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
Serial.print(") ");
// 24-hour with leading zeros
if (now.hour() < 10) Serial.print('0');
Serial.print(now.hour(), DEC);
Serial.print(':');
if (now.minute() < 10) Serial.print('0');
Serial.print(now.minute(), DEC);
Serial.print(':');
if (now.second() < 10) Serial.print('0');
Serial.print(now.second(), DEC);
Serial.print(" | ");
// -------------------------
// MPU read (ACCEL->TEMP->GYRO block starting at 0x3B)
// -------------------------
Wire.beginTransmission(MPU_I2C_SlaveAddress);
Wire.write(0x3B);
Wire.endTransmission(false);
Wire.requestFrom(MPU_I2C_SlaveAddress, 14, true);
accel_x = (Wire.read() << 8) | Wire.read();
accel_y = (Wire.read() << 8) | Wire.read();
accel_z = (Wire.read() << 8) | Wire.read();
temp    = (Wire.read() << 8) | Wire.read();
gyro_x  = (Wire.read() << 8) | Wire.read();
gyro_y  = (Wire.read() << 8) | Wire.read();
gyro_z  = (Wire.read() << 8) | Wire.read();
// Convert to friendly units (defaults assume +/-2g and +/-250 dps)
float ax_g = (float)accel_x / 16384.0f;
float ay_g = (float)accel_y / 16384.0f;
float az_g = (float)accel_z / 16384.0f;
float gx_dps = (float)gyro_x / 131.0f;
float gy_dps = (float)gyro_y / 131.0f;
float gz_dps = (float)gyro_z / 131.0f;
float tempF = (((float)temp) / 340.0f + 36.53f) * 1.8f + 32.0f;
// Print IMU data
Serial.print("Accel(");
Serial.print(ax_g, 2); Serial.print(",");
Serial.print(ay_g, 2); Serial.print(",");
Serial.print(az_g, 2); Serial.print(")");
Serial.print(" | Gyro(");
Serial.print(gx_dps, 2); Serial.print(",");
Serial.print(gy_dps, 2); Serial.print(",");
Serial.print(gz_dps, 2); Serial.print(")");
Serial.print(" | temp=");
Serial.print(tempF, 2);
Serial.println();
delay(50);  // faster updates than 100ms
}

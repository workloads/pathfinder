#ifndef BATTERY_H_
#define BATTERY_H_

#include <Arduino.h>
#include <INA219_WE.h>
#include <Wire.h>

// Battery address
extern const uint8_t batteryAddress;

// Battery Bit Mode
extern const BitMode batteryBitMode;

// Battery Power Gain (`PGain`)
extern const PGain batteryPGain;

// Battery Bus Voltage Range
extern const BRNG batteryBusVoltageRange;

// Battery Shunt Size
extern const float batteryShuntSize;

class BatteryData {
  public:
    float shuntVoltage    = 0.0;  // value in `mV`
    float busVoltage      = 0.0;  // value in `V`
    float current         = 0.0;  // value in `mV`
    float power           = 0.0;  // value in `mW`
    float loadVoltage     = 0.0;  // value in `V`
    bool  ina219_overflow = false;
};

class Battery {
  public:
    Battery();
    bool               setup();
    void               update();
    const BatteryData& getData() const;

  private:
    INA219_WE   ina219;
    BatteryData data;
};

#endif  // BATTERY_H_

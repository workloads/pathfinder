#include "Battery.h"
#include "Log.h"

// Battery address
const uint8_t batteryAddress = 0x42;

// Battery Bit Mode
//
// Resolution    Time      Mode
//  9 Bit         84 µs    BIT_MODE_9
// 10 Bit        148 µs    BIT_MODE_10
// 11 Bit        276 µs    BIT_MODE_11
// 12 Bit        532 µs    BIT_MODE_12  (default)
const BitMode batteryBitMode = BIT_MODE_9;

// Battery Power Gain (`PGain`)
//
// Gain      Shunt    max. current
//  40 mv    0.4 A    PG_40
//  80 mv    0.8 A    PG_80
// 160 mv    1.6 A    PG_160
// 320 mv    3.2 A    PG_320  (default)
const PGain batteryPGain = PG_320;

// Battery Bus Voltage Range
//
// 16 V = BRNG_16
// 32 V = BRNG_32  (default)
const BRNG batteryBusVoltageRange = BRNG_16;

// Battery Shunt Size
const float batteryShuntSize = 0.01;

Battery::Battery() : ina219(batteryAddress) {
}

bool Battery::setup() {
    const char *logTag = __func__;

    if (!ina219.init()) {
        logError(logTag, "Failed to initialize Battery");
        return false;
    }

    logInfo(logTag, "Configure Battery");
    ina219.setADCMode(batteryBitMode);
    ina219.setBusRange(batteryBusVoltageRange);
    ina219.setPGain(batteryPGain);
    ina219.setShuntSizeInOhms(batteryShuntSize);

    // Indicate that setup was successful
    return true;
}

void Battery::update() {
    const char *logTag = __func__;

    logInfo(logTag, "Updating Battery Data");

    data.shuntVoltage    = ina219.getShuntVoltage_mV();
    data.busVoltage      = ina219.getBusVoltage_V();
    data.current         = ina219.getCurrent_mA();
    data.power           = ina219.getBusPower();
    data.loadVoltage     = data.busVoltage + (data.shuntVoltage / 1000);
    data.ina219_overflow = ina219.getOverflow();

    logDebug(logTag, "Battery Data:");
    logDebug(logTag, "  Shunt:    %f mV", data.shuntVoltage);
    logDebug(logTag, "  Bus:      %f V", data.busVoltage);
    logDebug(logTag, "  Current:  %f mA", data.current);
    logDebug(logTag, "  Power:    %f mW", data.power);
    logDebug(logTag, "  Overflow: %d", data.ina219_overflow);
}

const BatteryData &Battery::getData() const {
    return data;
}

Battery battery;

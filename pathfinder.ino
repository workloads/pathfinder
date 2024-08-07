/**
 *
 * @file pathfinder.ino
 * @brief ESP32-based mobile edge computing platform
 * @url https://go.workloads.io/pathfinder
 * @version 0.9.5
 *
*/

#ifndef CORE_DEBUG_LEVEL
// Levels `INFO`, `DEBUG`, and `VERBOSE` may result in a performance decrease
#define CORE_DEBUG_LEVEL LOG_INFO

// TaskHandle_t Pid_ctrl;

#include <Adafruit_ICM20948.h>
#include <Adafruit_ICM20X.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <ESP32Encoder.h>
#include <INA219_WE.h>
#include <LittleFS.h>
#include <PID_v2.h>
#include <SCServo.h>
#include <SimpleKalmanFilter.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp_now.h>
#include <esp_system.h>
#include <math.h>
#include <nvs_flash.h>

StaticJsonDocument<256> jsonCmdReceive;
StaticJsonDocument<256> jsonInfoSend;
StaticJsonDocument<512> jsonInfoHttp;

// Load local configuration and helpers
#include "src/Battery.h"
#include "src/ugv_config.h"
#include "src/oled_ctrl.h"
#include "src/PinController.h"
#include "src/RoArm-M2_module.h"
#include "src/gimbal_module.h"
#include "src/json_cmd.h"
#include "src/IMU_ctrl.h"
#include "src/Movement.h"
#include "src/files_ctrl.h"
#include "src/ugv_advance.h"
#include "src/wifi_ctrl.h"
#include "src/esp_now_ctrl.h"
#include "src/uart_ctrl.h"
#include "src/HttpServer.h"

void moduleType_Gimbal() {
    getGimbalFeedback();
    gimbalSteady(steadyGoalY);
}

void setup() {
    Serial.begin(115200);
    Wire.begin(S_SDA, S_SCL);

    while (!Serial) {
    }

    ina219_init();
    inaDataUpdate();

    // set mainType & moduleType.
    // mainType: 1.WAVE ROVER, 2.UGV02, 3.UGV01
    // moduleType: 0.Null, 1.RoArm, 2.PT
    mm_settings(mainType, moduleType);

    init_oled();

    if (mainType == 1) {
        screenLine_0 = "WAVE ROVER";
    } else if (mainType == 2) {
        screenLine_0 = "UGV";
    } else if (mainType == 3) {
        screenLine_0 = "UGV";
    }

    screenLine_1 = "version: 0.95";
    screenLine_2 = "starting...";
    screenLine_3 = "";
    oled_update();

    delay(1200);

    // functions for IMU.
    imu_init();

    // functions for the LEDs on ugv.
    PinControllerInit();

    // init the littleFS functions in files_ctrl.h
    screenLine_2 = screenLine_3;
    screenLine_3 = "Initialize LittleFS";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("Initialize LittleFS for Flash files ctrl.");
    }

    initFS();

    // init the functions in switch_module.h
    screenLine_2 = screenLine_3;
    screenLine_3 = "Initialize 12V-switch ctrl";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("Initialize the pins used for 12V-switch ctrl.");
    }

    movtionPinInit();

    // servos power up
    screenLine_2 = screenLine_3;
    screenLine_3 = "Power up the servos";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("Power up the servos.");
    }

    delay(500);

    // init servo ctrl functions.
    screenLine_2 = screenLine_3;
    screenLine_3 = "ServoCtrl init UART2TTL...";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("ServoCtrl init UART2TTL...");
    }

    // check the status of the servos.
    screenLine_2 = screenLine_3;
    screenLine_3 = "Bus servos status check...";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("Bus servos status check...");
    }

    if (InfoPrint == 1) {
        Serial.println("All bus servos status checked.");
    }

    screenLine_3 = ">>> Moving to init pos...";
    oled_update();

    screenLine_3 = "Reset joint torque to ST_TORQUE_MAX";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("Reset joint torque to ST_TORQUE_MAX.");
    }

    screenLine_3 = "WiFi init";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("WiFi init.");
    }

    initWifi();

    screenLine_3 = "http & web init";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("http & web init.");
    }

    httpServerInit();

    screenLine_3 = "ESP-NOW init";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("ESP-NOW init.");
    }

    initEspNow();

    screenLine_3 = "IMU Calibrating";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("IMU Calibrating");
    }

    imuCalibration();

    screenLine_3 = "UGV started";
    oled_update();

    if (InfoPrint == 1) {
        Serial.println("UGV started.");
    }

    getThisDevMacAddress();

    updateOledWifiInfo();

    initEncoders();

    pidControllerInit();

    screenLine_2 = String("MAC:") + macToString(thisDevMac);
    oled_update();

    led_pwm_ctrl(0, 0);

    if (InfoPrint == 1) {
        Serial.println("Application initialization settings.");
    }

    createMission("boot", "these cmds run automatically at boot.");
    missionPlay("boot", 1);
}


// Enter application flow
void loop() {
    serialCtrl();
    server.handleClient();

    // read and compute the info of joints.
    switch (moduleType) {
        case 1:
            // RoArm 2
            break;

        case 2:
            moduleType_Gimbal();
            break;
    }

    // receive esp-now json cmd.
    if (runNewJsonCmd) {
        jsonCmdReceiveHandler();
        jsonCmdReceive.clear();
        runNewJsonCmd = false;
    }

    getLeftSpeed();

    LeftPidControllerCompute();

    getRightSpeed();

    RightPidControllerCompute();

    oledInfoUpdate();

    updateIMUData();

    if (baseFeedbackFlow) {
        baseInfoFeedback();
    }

    heartBeatCtrl();

    size_t freeHeap = esp_get_free_heap_size();
}

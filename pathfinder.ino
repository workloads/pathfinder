/**
 *
 * @file pathfinder.ino
 * @brief ESP32-based mobile edge computing platform
 * @url https://go.workloads.io/pathfinder
 * @version 1.3.5
 *
*/

#ifndef CORE_DEBUG_LEVEL
// Levels `INFO`, `DEBUG`, and `VERBOSE` may result in a performance decrease
#define CORE_DEBUG_LEVEL LOG_INFO
#endif

#include <ArduinoJson.h>
StaticJsonDocument<256> jsonCmdReceive;
StaticJsonDocument<256> jsonInfoSend;
StaticJsonDocument<1024> jsonInfoHttp;

DynamicJsonDocument apiResponse(1408);

#include <SCServo.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>
#include <esp_now.h>
#include <nvs_flash.h>
#include <Adafruit_SSD1306.h>
#include <INA219_WE.h>
#include <ESP32Encoder.h>
#include <PID_v2.h>
#include <SimpleKalmanFilter.h>
#include <math.h>
#include "ICM_20948.h"

// functions for barrery info.
#include "src/battery_ctrl.h"

// config for ugv.
#include "src/ugv_config.h"

// functions for oled.
#include "src/oled_ctrl.h"

// functions for the leds of UGV.
#include "src/ugv_led_ctrl.h"


// define json cmd.
#include "src/json_cmd.h"

// functions for IMU ctrl.
#include "src/IMU_ctrl.h"

// functions for movtion ctrl. 
#include "src/movtion_module.h"

// functions for editing the files in flash.
#include "src/files_ctrl.h"

// advance functions for ugv ctrl.
#include "src/ugv_advance.h"

// functions for wifi ctrl.
#include "src/wifi_ctrl.h"

// functions for API.
#include "src/api_ctrl.h"

// functions for esp-now.
#include "src/esp_now_ctrl.h"

// functions for uart json ctrl.
#include "src/uart_ctrl.h"

// functions for http & web server.
#include "src/http_server.h"



void setup() {
  Serial.begin(115200);
  while(!Serial) {}
  Wire.begin(S_SDA, S_SCL, 400000);

  bool initialized = false;
  while (!initialized) {
    myICM.begin(Wire, AD0_VAL);
    Serial.print(F("Initialization of the sensor returned: "));
    Serial.println(myICM.statusString());
    if (myICM.status != ICM_20948_Stat_Ok) {
      Serial.println(F("Trying again..."));
      delay(500);
    }
    else {
      initialized = true;
    }
  }

  Serial.println(F("Device connected!"));

  bool success = true; // Use success to show if the DMP configuration was successful

  // Initialize the DMP. initializeDMP is a weak function. You can overwrite it if you want to e.g. to change the sample rate
  success &= (myICM.initializeDMP() == ICM_20948_Stat_Ok);

  // Enable the DMP orientation sensor
  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_ORIENTATION) == ICM_20948_Stat_Ok);

  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_GYROSCOPE) == ICM_20948_Stat_Ok);
  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_ACCELEROMETER) == ICM_20948_Stat_Ok);
  // +
  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED) == ICM_20948_Stat_Ok);

  // Configuring DMP to output data at multiple ODRs:
  // DMP is capable of outputting multiple sensor data at different rates to FIFO.
  // Setting value can be calculated as follows:
  // Value = (DMP running rate / ODR ) - 1
  // E.g. For a 5Hz ODR rate when DMP is running at 55Hz, value = (55/5) - 1 = 10.

  // success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat9, 3) == ICM_20948_Stat_Ok); // Set to the maximum

  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Accel, 3) == ICM_20948_Stat_Ok); // Set to the maximum
  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Gyro, 3) == ICM_20948_Stat_Ok); // Set to the maximum

  // +
  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Cpass, 3) == ICM_20948_Stat_Ok);
  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Cpass_Calibr, 3) == ICM_20948_Stat_Ok);

  myICM.lowPower(false);

  // Enable the FIFO
  success &= (myICM.enableFIFO() == ICM_20948_Stat_Ok);

  // Enable the DMP
  success &= (myICM.enableDMP() == ICM_20948_Stat_Ok);

  // Reset DMP
  success &= (myICM.resetDMP() == ICM_20948_Stat_Ok);

  // Reset FIFO
  success &= (myICM.resetFIFO() == ICM_20948_Stat_Ok);

  // Check success
  if (success)
  {
    Serial.println(F("DMP enabled!"));
  }
  else
  {
    Serial.println(F("Enable DMP failed!"));
    Serial.println(F("Please check that you have uncommented line 29 (#define ICM_20948_USE_DMP) in ICM_20948_C.h..."));
  }

  ina219_init();
  inaDataUpdate();

  // set mainType & moduleType.
  // mainType: 1.RaspRover, 2.UGV Rover, 3.UGV Beast
  // moduleType: 0.Null, 2.PT
  mm_settings(mainType, moduleType);

  init_oled();
  screenLine_0 = "Pathfinder";
  screenLine_1 = "version: 1.14";
  screenLine_2 = "starting...";
  screenLine_3 = "";
  oled_update();

  delay(1200);

  // functions for IMU.
  // imu_init();

  // functions for the leds on ugv.
  led_pin_init();

  // init the littleFS funcs in files_ctrl.h
  screenLine_2 = screenLine_3;
  screenLine_3 = "Initialize LittleFS";
  oled_update();
  if(InfoPrint == 1){Serial.println("Initialize LittleFS for Flash files ctrl.");}
  initFS();

  // init the funcs in switch_module.h
  screenLine_2 = screenLine_3;
  screenLine_3 = "Initialize 12V-switch ctrl";
  oled_update();
  if(InfoPrint == 1){Serial.println("Initialize the pins used for 12V-switch ctrl.");}
  movtionPinInit();

  // servos power up
  screenLine_2 = screenLine_3;
  screenLine_3 = "Power up the servos";
  oled_update();
  if(InfoPrint == 1){Serial.println("Power up the servos.");}
  delay(500);
  

  screenLine_3 = "WiFi init";
  oled_update();
  if(InfoPrint == 1){Serial.println("WiFi init.");}
  initWifi();

  screenLine_3 = "http & web init";
  oled_update();
  if(InfoPrint == 1){Serial.println("http & web init.");}
  initHttpWebServer();

  screenLine_3 = "ESP-NOW init";
  oled_update();
  if(InfoPrint == 1){Serial.println("ESP-NOW init.");}
  initEspNow();

  screenLine_3 = "IMU Calibrating";
  oled_update();
  if(InfoPrint == 1){Serial.println("IMU Calibrating");}
  imuCalibration();

  screenLine_3 = "UGV started";
  oled_update();
  if(InfoPrint == 1){Serial.println("UGV started.");}

  getThisDevMacAddress();

  updateOledWifiInfo();

  initEncoders();

  pidControllerInit();

  screenLine_2 = String("MAC:") + macToString(thisDevMac);
  oled_update();

  led_pwm_ctrl(0, 0);

  if(InfoPrint == 1){Serial.println("Application initialization settings.");}
  createMission("boot", "these cmds run automatically at boot.");
  missionPlay("boot", 1);
}


void loop() {
  icm_20948_DMP_data_t data;
  myICM.readDMPdataFromFIFO(&data);

  if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) {
    // if ((data.header & DMP_header_bitmap_Quat9) > 0) {
    //   // Scale to +/- 1
    //   q1 = ((double)data.Quat9.Data.Q1) / 1073741824.0; // Convert to double. Divide by 2^30
    //   q2 = ((double)data.Quat9.Data.Q2) / 1073741824.0; // Convert to double. Divide by 2^30
    //   q3 = ((double)data.Quat9.Data.Q3) / 1073741824.0; // Convert to double. Divide by 2^30
    //   q0 = sqrt(1.0 - ((q1 * q1) + (q2 * q2) + (q3 * q3)));

    //   q2sqr = q2 * q2;

    //   // roll (x-axis rotation)
    //   t0 = +2.0 * (q0 * q1 + q2 * q3);
    //   t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
    //   icm_roll = atan2(t0, t1);

    //   // pitch (y-axis rotation)
    //   t2 = +2.0 * (q0 * q2 - q3 * q1);
    //   t2 = t2 > 1.0 ? 1.0 : t2;
    //   t2 = t2 < -1.0 ? -1.0 : t2;
    //   icm_pitch = asin(t2);

    //   // yaw (z-axis rotation)
    //   t3 = +2.0 * (q0 * q3 + q1 * q2);
    //   t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
    //   icm_yaw = atan2(t3, t4);

    //   // Serial.print(F("r:"));
    //   // Serial.print(icm_roll, 2);
    //   // Serial.print(F(" p:"));
    //   // Serial.print(icm_pitch, 2);
    //   // Serial.print(F(" y:"));
    //   // Serial.println(icm_yaw, 2);
    // }

    if ((data.header & DMP_header_bitmap_Accel) > 0) {
      ax = data.Raw_Accel.Data.X;
      ay = data.Raw_Accel.Data.Y;
      az = data.Raw_Accel.Data.Z;
    }
    if ((data.header & DMP_header_bitmap_Gyro) > 0) {
      gx = data.Raw_Gyro.Data.X;
      gy = data.Raw_Gyro.Data.Y;
      gz = data.Raw_Gyro.Data.Z;
    }
    if ((data.header & DMP_header_bitmap_Compass) > 0) {
      mx = data.Compass.Data.X;
      my = data.Compass.Data.Y;
      mz = data.Compass.Data.Z;
    }
  }

  serialCtrl();
  server.handleClient();

  // read and compute the info of joints.
  // No additional modules (gimbal functionality removed)

  // recv esp-now json cmd.
  if(runNewJsonCmd) {
    jsonCmdReceiveHandler();
    jsonCmdReceive.clear();
    runNewJsonCmd = false;
  }

  getLeftSpeed();

  LeftPidControllerCompute();
  
  getRightSpeed();
  
  RightPidControllerCompute();
  
  oledInfoUpdate();

  // Printing feedback about position just spams the serial
  //if (baseFeedbackFlow) {
  //  baseInfoFeedback();
  //}

  heartBeatCtrl();
}

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
JsonDocument jsonCmdReceive;
JsonDocument jsonInfoSend;
JsonDocument jsonInfoHttp;

JsonDocument apiResponse;

#include <SCServo.h>
#include <nvs_flash.h>
#include <esp_system.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <WebServer.h>
#include <nvs_flash.h>
#include <Adafruit_SSD1306.h>
#include <INA219_WE.h>
#include <ESP32Encoder.h>
#include <PID_v2.h>
#include <SimpleKalmanFilter.h>
#include <math.h>
#include "ICM_20948.h"

#include "src/battery.h"
#include "src/config.h"
#include "src/oled.h"
#include "src/led.h"
#include "src/json.h"
#include "src/imu.h"
#include "src/motion.h"
#include "src/files.h"
#include "src/extras.h"
#include "src/wifi.h"
#include "src/api.h"
#include "src/api_v2.h"
#include "src/uart.h"
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
  if (InfoPrint == 1) {
    Serial.print("Orientation sensor: ");
    Serial.println(myICM.statusString());
  }

  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_GYROSCOPE) == ICM_20948_Stat_Ok);
  if (InfoPrint == 1) {
    Serial.print("Raw gyroscope sensor: ");
    Serial.println(myICM.statusString());
  }
  
  success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_RAW_ACCELEROMETER) == ICM_20948_Stat_Ok);
  if (InfoPrint == 1) {
    Serial.print("Raw accelerometer sensor: ");
    Serial.println(myICM.statusString());
  }
  
  // +
  // Temporarily disable magnetometer sensor to focus on accel/gyro
  // success &= (myICM.enableDMPSensor(INV_ICM20948_SENSOR_MAGNETIC_FIELD_UNCALIBRATED) == ICM_20948_Stat_Ok);
  if (InfoPrint == 1) {
    Serial.println("Magnetometer sensor: DISABLED for now");
  }

  // Configuring DMP to output data at multiple ODRs:
  // DMP is capable of outputting multiple sensor data at different rates to FIFO.
  // Setting value can be calculated as follows:
  // Value = (DMP running rate / ODR ) - 1
  // E.g. For a 5Hz ODR rate when DMP is running at 55Hz, value = (55/5) - 1 = 10.

  success &= (myICM.setDMPODRrate(DMP_ODR_Reg_Quat9, 3) == ICM_20948_Stat_Ok); // Set to the maximum

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
  if (success) {
    Serial.println(F("DMP enabled!"));
  } else {
    Serial.println(F("Enable DMP failed!"));
    Serial.println(F("Please check that you have uncommented line 29 (#define ICM_20948_USE_DMP) in ICM_20948_C.h..."));
    Serial.print(F("DMP initialization status: "));
    Serial.println(myICM.statusString());
  }

  ina219_init();
  inaDataUpdate();

  // set mainType & moduleType.
  // mainType: 1.RaspRover, 2.UGV Rover, 3.UGV Beast
  // moduleType: 0.Null, 2.PT
  mm_settings(mainType, moduleType);

  init_oled();
  screenLine_0 = "Pathfinder";
  screenLine_1 = "version: 1.3.6";
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
  
  if (InfoPrint == 1) {
    Serial.println("Initialize LittleFS for Flash files ctrl.");
  }
  
  initFS();

  // init the funcs in switch_module.h
  screenLine_2 = screenLine_3;
  screenLine_3 = "Initialize 12V-switch ctrl";
  oled_update();

  if (InfoPrint == 1) {
    Serial.println("Initialize the pins used for 12V-switch ctrl.");
  }
  
  motionPinInit();

  // servos power up
  screenLine_2 = screenLine_3;
  screenLine_3 = "Power up the servos";
  oled_update();
  
  if (InfoPrint == 1) {
    Serial.println("Power up the servos.");
  }
  
  delay(500);

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
  
  initHttpWebServer();

  screenLine_3 = "IMU Calibrating";
  oled_update();
  
  if (InfoPrint == 1) {
    Serial.println("IMU Calibrating");
  }
  
  imuCalibration();

  screenLine_3 = "Pathfinder started";
  oled_update();
  
  if (InfoPrint == 1) {
    Serial.println("Pathfinder started.");
  }

  updateOledWifiInfo();

  initEncoders();

  pidControllerInit();

  screenLine_2 = "Pathfinder ready.";
  oled_update();

  led_pwm_ctrl(0, 0);

  if (InfoPrint == 1) {
    Serial.println("Application initialization settings.");
  }
  
  createMission("boot", "these cmds run automatically at boot.");
  missionPlay("boot", 1);
}

void loop() {
  icm_20948_DMP_data_t data;
  myICM.readDMPdataFromFIFO(&data);

  if ((myICM.status == ICM_20948_Stat_Ok) || (myICM.status == ICM_20948_Stat_FIFOMoreDataAvail)) {
    // Debug: Print which data types are available
    if (InfoPrint == 1) {
      Serial.print("DMP Header: 0x");
      Serial.print(data.header, HEX);
      Serial.print(" - Quat9: ");
      Serial.print((data.header & DMP_header_bitmap_Quat9) > 0 ? "YES" : "NO");
      Serial.print(" Accel: ");
      Serial.print((data.header & DMP_header_bitmap_Accel) > 0 ? "YES" : "NO");
      Serial.print(" Gyro: ");
      Serial.print((data.header & DMP_header_bitmap_Gyro) > 0 ? "YES" : "NO");
      Serial.print(" Compass: ");
      Serial.println((data.header & DMP_header_bitmap_Compass) > 0 ? "YES" : "NO");
    }
    
    // Try to read raw sensor data directly (not from DMP)
    if (myICM.dataReady()) {
      myICM.getAGMT();
      ax = myICM.agmt.acc.axes.x;
      ay = myICM.agmt.acc.axes.y;
      az = myICM.agmt.acc.axes.z;
      gx = myICM.agmt.gyr.axes.x;
      gy = myICM.agmt.gyr.axes.y;
      gz = myICM.agmt.gyr.axes.z;
      mx = myICM.agmt.mag.axes.x;
      my = myICM.agmt.mag.axes.y;
      mz = myICM.agmt.mag.axes.z;
      
      // Calculate roll, pitch, yaw from raw accelerometer data
      // Convert to m/s^2 using configurable scale factor
      double accel_x = ax / IMU_ACCEL_SCALE_FACTOR;
      double accel_y = ay / IMU_ACCEL_SCALE_FACTOR;
      double accel_z = az / IMU_ACCEL_SCALE_FACTOR;
      
      // Calculate roll and pitch from accelerometer
      icm_roll = atan2(accel_y, sqrt(accel_x * accel_x + accel_z * accel_z));
      icm_pitch = atan2(-accel_x, sqrt(accel_y * accel_y + accel_z * accel_z));
      
      // For yaw, we would need magnetometer data, but for now set to 0
      // icm_yaw = atan2(mx * cos(icm_pitch) + my * sin(icm_roll) * sin(icm_pitch) + mz * cos(icm_roll) * sin(icm_pitch),
      //                  my * cos(icm_roll) - mz * sin(icm_roll));
      icm_yaw = 0.0; // Set to 0 for now
      
      // Check IMU safety - stop movement if pitch exceeds limit
      checkIMUSafety();
      
      if (InfoPrint == 1) {
        Serial.print("Raw Accel: X=");
        Serial.print(ax);
        Serial.print(" Y=");
        Serial.print(ay);
        Serial.print(" Z=");
        Serial.println(az);
        Serial.print("Raw Gyro: X=");
        Serial.print(gx);
        Serial.print(" Y=");
        Serial.print(gy);
        Serial.print(" Z=");
        Serial.println(gz);
        Serial.print("Raw Mag: X=");
        Serial.print(mx);
        Serial.print(" Y=");
        Serial.print(my);
        Serial.print(" Z=");
        Serial.println(mz);
        Serial.print("Calculated Roll: ");
        Serial.print(icm_roll * 180.0 / PI, 2);
        Serial.print("° Pitch: ");
        Serial.print(icm_pitch * 180.0 / PI, 2);
        Serial.print("° Yaw: ");
        Serial.print(icm_yaw * 180.0 / PI, 2);
        Serial.println("°");
      }
    }
    if ((data.header & DMP_header_bitmap_Quat9) > 0) {
      // Scale to +/- 1
      // Convert to double. Divide by 2^30
      q1 = ((double)data.Quat9.Data.Q1) / 1073741824.0; 
      q2 = ((double)data.Quat9.Data.Q2) / 1073741824.0;
      q3 = ((double)data.Quat9.Data.Q3) / 1073741824.0;
      double quat_sum_sq = (q1 * q1) + (q2 * q2) + (q3 * q3);
      if (quat_sum_sq <= 1.0) {
        q0 = sqrt(1.0 - quat_sum_sq);
      } else {
        q0 = 0.0; // Handle invalid quaternion
      }

      if (InfoPrint == 1) {
        Serial.print("Raw Quat: Q0=");
        Serial.print(q0, 6);
        Serial.print(" Q1=");
        Serial.print(q1, 6);
        Serial.print(" Q2=");
        Serial.print(q2, 6);
        Serial.print(" Q3=");
        Serial.println(q3, 6);
        Serial.print("Quat sum sq: ");
        Serial.println(quat_sum_sq, 6);
      }

      // Check for valid quaternion before calculating angles
      if (!isnan(q0) && !isnan(q1) && !isnan(q2) && !isnan(q3)) {
        q2sqr = q2 * q2;

        // roll (x-axis rotation)
        t0 = +2.0 * (q0 * q1 + q2 * q3);
        t1 = +1.0 - 2.0 * (q1 * q1 + q2sqr);
        icm_roll = atan2(t0, t1);

        // pitch (y-axis rotation)
        t2 = +2.0 * (q0 * q2 - q3 * q1);
        t2 = t2 > 1.0 ? 1.0 : t2;
        t2 = t2 < -1.0 ? -1.0 : t2;
        icm_pitch = asin(t2);

        // yaw (z-axis rotation)
        t3 = +2.0 * (q0 * q3 + q1 * q2);
        t4 = +1.0 - 2.0 * (q2sqr + q3 * q3);
        icm_yaw = atan2(t3, t4);
      } else {
        icm_roll = 0.0;
        icm_pitch = 0.0;
        icm_yaw = 0.0;
        if (InfoPrint == 1) {
          Serial.println("Invalid quaternion - setting angles to 0");
        }
      }

      Serial.print(F("r:"));
      Serial.print(icm_roll, 2);
      Serial.print(F(" p:"));
      Serial.print(icm_pitch, 2);
      Serial.print(F(" y:"));
      Serial.println(icm_yaw, 2);
    }

    if ((data.header & DMP_header_bitmap_Accel) > 0) {
      ax = data.Raw_Accel.Data.X;
      ay = data.Raw_Accel.Data.Y;
      az = data.Raw_Accel.Data.Z;
      if (InfoPrint == 1) {
        Serial.print("Accel: X=");
        Serial.print(ax);
        Serial.print(" Y=");
        Serial.print(ay);
        Serial.print(" Z=");
        Serial.println(az);
      }
    }
    if ((data.header & DMP_header_bitmap_Gyro) > 0) {
      gx = data.Raw_Gyro.Data.X;
      gy = data.Raw_Gyro.Data.Y;
      gz = data.Raw_Gyro.Data.Z;
      if (InfoPrint == 1) {
        Serial.print("Gyro: X=");
        Serial.print(gx);
        Serial.print(" Y=");
        Serial.print(gy);
        Serial.print(" Z=");
        Serial.println(gz);
      }
    }
    // Temporarily disable magnetometer data reading
    /*
    if ((data.header & DMP_header_bitmap_Compass) > 0) {
      mx = data.Compass.Data.X;
      my = data.Compass.Data.Y;
      mz = data.Compass.Data.Z;
      if (InfoPrint == 1) {
        Serial.print("Compass: X=");
        Serial.print(mx);
        Serial.print(" Y=");
        Serial.print(my);
        Serial.print(" Z=");
        Serial.println(mz);
      }
    }
    */
  }

  serialCtrl();
  server.handleClient();

  // Check WiFi status and attempt reconnection if needed
  checkWifiAndReconnect();

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

  // Send sensor data to web interface
  if (baseFeedbackFlow) {
    baseInfoFeedback();
  }

  heartBeatCtrl();
}

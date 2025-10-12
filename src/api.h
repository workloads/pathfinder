#if API_V1
String jsonAPIHandler(String stringInput)
{
  JsonDocument jsonInput;
  deserializeJson(jsonInput, stringInput);
  int cmdType = jsonInput["T"].as<int>();
  switch (cmdType)
  {
	// emergency stop.
	case CMD_EMERGENCY_STOP:
		setGoalSpeed(0, 0);
		break;

  case CMD_SPEED_CTRL:
    if (jsonInput["T"].is<int>())
    {
      if (jsonInput["L"].is<float>() && jsonInput["R"].is<float>())
      {
        heartbeatStopFlag = false;
        lastCmdRecvTime = millis();
        setAPIGoalSpeed(jsonInput["L"], jsonInput["R"]);
      }
    }
    break;

  case CMD_PWM_INPUT:
    usePIDCompute = false;
    heartbeatStopFlag = false;
    lastCmdRecvTime = millis();
    leftCtrl(jsonInput["L"]);
    rightCtrl(jsonInput["R"]);
    break;

  case CMD_ROS_CTRL:
    rosCtrl(
        jsonInput["X"],
        jsonInput["Z"]);

    heartbeatStopFlag = false;
    lastCmdRecvTime = millis();
    break;

  case CMD_SET_MOTOR_PID:
    setPID(
        jsonInput["P"],
        jsonInput["I"],
        jsonInput["D"],
        jsonInput["L"]);
    break;

  case CMD_OLED_CTRL:
    oledCtrl(
        jsonInput["lineNum"],
        jsonInput["Text"]);
    break;

  case CMD_OLED_DEFAULT:
    setOledDefault();
    break;

  case CMD_MODULE_TYPE:
    changeModuleType(jsonInput["cmd"]);
    break;

  case CMD_GET_IMU_DATA:
    getIMUData();
    break;

  case CMD_CALI_IMU_STEP:
    imuCalibration();
    break;

  case CMD_GET_IMU_OFFSET:
    getIMUOffset();
    break;

  case CMD_SET_IMU_OFFSET:
    setIMUOffset(
			jsonInput["gx"],
			jsonInput["gy"],
			jsonInput["gz"],
			jsonInput["ax"],
			jsonInput["ay"],
			jsonInput["az"],
			jsonInput["cx"],
			jsonInput["cy"],
			jsonInput["cz"]);
    break;

  case CMD_BASE_FEEDBACK:
    baseInfoFeedbackHttp();
    break;
  #if BASE_FEEDBACK_FLOW
  case CMD_BASE_FEEDBACK_FLOW:
    setBaseInfoFeedbackMode(jsonInput["cmd"]);
    break;
  #endif

  case CMD_FEEDBACK_FLOW_INTERVAL:
    setFeedbackFlowInterval(jsonInput["cmd"]);
    break;

  case CMD_UART_ECHO_MODE:
    setCmdEcho(jsonInput["cmd"]);
    break;

  case CMD_IMU_SAFETY_STATUS:
    getIMUSafetyStatus();
    break;


  case CMD_LED_CTRL:
    led_pwm_ctrl(
        jsonInput["IO4"],
        jsonInput["IO5"]);

    break;

  case CMD_HEART_BEAT_SET:
    changeHeartBeatDelay(
        jsonInput["cmd"]);
    break;

  case CMD_SET_SPD_RATE:
    setSpdRate(
        jsonInput["L"],
        jsonInput["R"]);
    break;

  case CMD_GET_SPD_RATE:
    getSpdRate();
    break;

  case CMD_SAVE_SPD_RATE:
    saveSpdRate();
    break;

  // EoAT type settings.
  case CMD_EOAT_TYPE:
    configEEmodeType(
        jsonInput["mode"]);
    break;

    case CMD_CONFIG_EOAT:
    configEoAT(
        jsonInput["pos"],
        jsonInput["ea"],
        jsonInput["eb"]);
    break;

  case CMD_SWITCH_OFF:
    switchEmergencyStop();
    break;
    // constant ctrl
    // m: 0 - angle
    //    1 - xyzt
    // cmd: 0 - stop
    // 		  1 - increase
    // 		  2 - decrease
    // {"T":123,"m":0,"axis":0,"cmd":0,"spd":0}

  // mission & steps edit & file edit.
  case CMD_SCAN_FILES:
    scanFlashContents();
    break;
  case CMD_CREATE_FILE:
    createFile(
        jsonInput["name"],
        jsonInput["content"]);
    break;
  case CMD_READ_FILE:
    readFile(
        jsonInput["name"]);
    break;
  case CMD_DELETE_FILE:
    deleteFile(
        jsonInput["name"]);
    break;

  case CMD_APPEND_LINE:
    appendLine(
        jsonInput["name"],
        jsonInput["content"]);
    break;

  case CMD_INSERT_LINE:
    insertLine(
        jsonInput["name"],
        jsonInput["lineNum"],
        jsonInput["content"]);
    break;

  case CMD_REPLACE_LINE:
    replaceLine(
        jsonInput["name"],
        jsonInput["lineNum"],
        jsonInput["content"]);
    break;

  case CMD_READ_LINE:
    readSingleLine(
        jsonInput["name"],
        jsonInput["lineNum"]);
    break;

  case CMD_DELETE_LINE:
    deleteSingleLine(
        jsonInput["name"],
        jsonInput["lineNum"]);
    break;


  case CMD_CREATE_MISSION:
    createMission(
        jsonInput["name"],
        jsonInput["intro"]);
    break;

  case CMD_MISSION_CONTENT:
    missionContent(
        jsonInput["name"]);
    break;

  case CMD_APPEND_STEP_JSON:
    appendStepJson(
        jsonInput["name"],
        jsonInput["step"]);
    break;

  case CMD_APPEND_STEP_FB:
    appendStepFB(
        jsonInput["name"],
        jsonInput["spd"]);
    break;

  case CMD_APPEND_DELAY:
    appendDelayCmd(
        jsonInput["name"],
        jsonInput["delay"]);
    break;

  case CMD_INSERT_STEP_JSON:
    insertStepJson(
        jsonInput["name"],
        jsonInput["stepNum"],
        jsonInput["step"]);
    break;

  case CMD_INSERT_STEP_FB:
    insertStepFB(
        jsonInput["name"],
        jsonInput["stepNum"],
        jsonInput["spd"]);
    break;

  case CMD_INSERT_DELAY:
    insertDelayCmd(
        jsonInput["name"],
        jsonInput["stepNum"],
        jsonInput["spd"]);
    break;

  case CMD_REPLACE_STEP_JSON:
    replaceStepJson(
        jsonInput["name"],
        jsonInput["stepNum"],
        jsonInput["step"]);
    break;

  case CMD_REPLACE_STEP_FB:
    replaceStepFB(
        jsonInput["name"],
        jsonInput["stepNum"],
        jsonInput["spd"]);
    break;
  case CMD_REPLACE_DELAY:
    replaceDelayCmd(
        jsonInput["name"],
        jsonInput["stepNum"],
        jsonInput["delay"]);
    break;
  case CMD_DELETE_STEP:
    deleteStep(
        jsonInput["name"],
        jsonInput["stepNum"]);
    break;

  case CMD_MOVE_TO_STEP:
    moveToStep(
        jsonInput["name"],
        jsonInput["stepNum"]);
    break;
  case CMD_MISSION_PLAY:
    missionPlay(
        jsonInput["name"],
        jsonInput["times"]);
    break;

  // esp-32 dev ctrl.
  case CMD_REBOOT:
    esp_restart();
    break;
  case CMD_FREE_FLASH_SPACE:
    freeFlashSpace();
    break;
  case CMD_BOOT_MISSION_INFO:
    missionContent("boot");
    break;
  case CMD_RESET_BOOT_MISSION:
    deleteFile("boot.mission");
    createFile("boot", "these cmds run automatically at boot.");
    break;

  case CMD_NVS_CLEAR:
    nvs_flash_erase();
    delay(1000);
    nvs_flash_init();
    break;
  #if INFO_PRINT
    case CMD_INFO_PRINT:
      configInfoPrint(
          jsonInput["cmd"]);
      break;
  #endif
  // case CMD_PID_RESET_A: PID_v2 pidA(__kp, __ki, __kd, PID::Direct);
  // 											PID_v2 pidB(__kp, __ki, __kd, PID::Direct);
  // 											pidControllerInit();break;

  // mainType & moduleType settings.
  case CMD_MM_TYPE_SET:
    mm_settings(jsonInput["main"], jsonInput["module"]);
    break;
  }
  return "OK";
}

String apiStopDevice()
{
  String command = "{\"T\":1,\"L\":0,\"R\":0}";
  delay(100);
  Serial.println("stop-A");
  Serial.println(command);
  jsonAPIHandler("{\"T\":1,\"L\":0,\"R\":0}");
  Serial.println("stop-B");
  Serial.println(command);
  delay(100);
  return "OK";
}

const unsigned long AUTO_STOP_DELAY = 3000;
unsigned long lastCommandTime = 0;

void planStepToCommand(String movementPlanStep)
{
  JsonDocument step;
  deserializeJson(step, movementPlanStep);

  int angle = step["angle"].as<int>();
  int distance = step["distance"].as<int>();
  String direction = step["direction"].as<String>();

  // Constants
  const float speed = 0.5;
  const int spinDelay = 2000;       // 2 seconds delay before spinning
  const int msPerDegree = 800 / 90; // ms needed for 1 degree of rotation

  Serial.println("Movement Plan Step: " + movementPlanStep);
  Serial.println("angle: " + String(angle));
  Serial.println("Distance: " + String(distance));
  Serial.println("Direction: " + direction);

  // Step 1: Spin according to angle
  if (angle != 0)
  {
    String rotationCommand;
    int rotationDuration;

    // Wait before spinning
    delay(spinDelay);

    if (angle > 0 && angle <= 180)
    {
      // Spin right
      rotationCommand = "{\"T\":1, \"L\":" + String(speed) + ", \"R\":-" + String(speed) + "}";
      rotationDuration = angle * msPerDegree;
    }
    else if (angle > 180 && angle < 360)
    {
      // Spin left
      rotationCommand = "{\"T\":1, \"L\":-" + String(speed) + ", \"R\":" + String(speed) + "}";
      rotationDuration = (360 - angle) * msPerDegree;
    }
    else
    {
      Serial.println("Invalid angle value");
      return;
    }

    // Perform rotation
    Serial.println("Rotation command: " + rotationCommand);
    Serial.println("Rotation duration: " + String(rotationDuration) + " ms");
    jsonAPIHandler(rotationCommand);
    delay(rotationDuration);

    // Stop rotation
    Serial.println("Stopping rotation");
    jsonAPIHandler("{\"T\":1, \"L\":0, \"R\":0}");
    delay(100); // Short pause after rotation
  }

  // Step 2: Move in the specified direction for the given distance
  String movementCommand;
  if (direction == "forward")
  {
    movementCommand = "{\"T\":1, \"L\":" + String(speed) + ", \"R\":" + String(speed) + "}";
  }
  else if (direction == "backward")
  {
    movementCommand = "{\"T\":1, \"L\":-" + String(speed) + ", \"R\":-" + String(speed) + "}";
  }
  else
  {
    Serial.println("Invalid direction");
    return;
  }

  // Perform movement
  Serial.println("Movement command: " + movementCommand);
  jsonAPIHandler(movementCommand);

  // Calculate total movement time
  unsigned long movementDuration = 100UL * distance; // 100ms per unit of distance
  unsigned long startTime = millis();

  while (millis() - startTime < movementDuration)
  {
    Serial.println(startTime);
    Serial.println(movementDuration);
    Serial.println(AUTO_STOP_DELAY);
    // Check if we need to send a new command to prevent auto-stop
    if (millis() - lastCommandTime >= AUTO_STOP_DELAY - 100)
    { // Send command 100ms before auto-stop
      jsonAPIHandler(movementCommand);
      lastCommandTime = millis();
    }
    delay(10); // Small delay to prevent excessive looping
  }

  // Stop movement
  Serial.println("Stopping movement");
  jsonAPIHandler("{\"T\":1, \"L\":0, \"R\":0}");
  lastCommandTime = millis();
}

// void planStepToCommand(String movementPlanStep) {
// 	// 1 degree = 0.017453 rad
// 	// useful for converting the angle from the movement plan from degrees to radians
// 	// which the Rover accepts
// 	float degreeToRad = 0.017453;

// 	float rotationDuration = 1500;
// 	String rotationDirection = "";

// 	int angle = 0;
//   int distance = 0;
// 	// rotation speed in degrees per ms with T:1 and L:0.5 or R:0.5
// 	float degreesPerMs = 0.15;

// 	// Speed
// 	String speed = "0.5";

// 	// Half rotation time (time it takes to rotate 180 degrees)
// 	//
// 	String rotationTime = "";

//     String command = "";
//     String direction = "";
//     JsonDocument step;

//     /*
//     else if direction == "left" {
//         command = '{"T":1,"L":-1,"R":1}';
//     }
//     else if direction == "right" {
//         command = '{"T":1,"L":1,"R":-1}';
//     }
//     */
//    Serial.println(movementPlanStep);
//    deserializeJson(step, movementPlanStep);
//    	// Check if we need to switch angle first
// 	angle = step["angle"].as<int>();
//   distance = step["distance"].as<int>();
// 	Serial.println(angle);
//     if (angle != 0) {
// 		if (angle > 0 && angle < 180) {
// 			// rotationDuration = degreesPerMs * (float)angle;
// 			rotationDirection = "right";
// 			// this needs to be updated to use tthe float speed above
// 			command = "{\"T\":1, \"L\":0, \"R\":0.5}";
// 			Serial.println(command);

// 		//	rotation = step["angle"].as<int>() * degreeToRad / 3 ;
// 		//	command = '{"T":13,"X":' + speed + ',"Z":' + rotation  + '}';
// 		}
// 		else if (angle > 180 && angle < 360) {
// 			// rotationDuration = degreesPerMs * angle;
// 			rotationDirection = "left";
// 			// this needs to be updated to use tthe float speed above
// 			command = "{\"T\":1, \"L\":0.5, \"R\":0}";
// 					Serial.println(command);
// 		}
// 		else {
// 			Serial.println("Invalid angle value");// + String(step["angle"]));
// 			return;

//         //command = '{"T":1,"L":0,"R":0}';
// 		// pseudocode
// 		// if 2s are needed for 90 degrees rotation then 1s are needed for 45 degrees rotation
// 		// perform rotation
// 		Serial.println(command);
//     jsonAPIHandler(command);
//     // Serial.println(rotationDuration);
// 		// delay((int)rotationDuration);
//     delay(1000);
//     Serial.println("delay finish");
// 		// stop rotation
// 		// apiStopDevice();

//     command = "{\"T\":1, \"L\":0.5, \"R\":0.5}";
//     Serial.println(command);
//     delay(10*distance);
//     }

//     // if (step["direction"] == "forward") {
//     //     //command = '{"T":1,"L":1,"R":1}';
//     //     direction = "+";
//     // }
//     // else if (step["direction"] == "backward") {
//     //     //command = '{"T":1,"L":-1,"R":-1}';
//     //     direction = "-";
//     // }

//     // command = '{"T":1';
//     //after rotated, move distance as defined, coupled with direction from above
//     //jsonAPIHandler(command);
// }
// }



String apiMovementPlanHandler(String jsonInput)
{
  JsonDocument input;

  // parse the JSON file using ArduinoJson.
  deserializeJson(input, jsonInput);
  Serial.println("Received a movement plan"); //, String(input["name"]));
                                              //    Serial.println(input);
  // for (int i = 0; i < input["steps"].size(); i++) {
  JsonArray steps = input["steps"].as<JsonArray>();
  String checkResult = checkPlanSchema(steps);
  if (checkResult != "valid")
  {
    return checkResult;
  }
  for (String i : steps)
  {
    Serial.println("Step");
    planStepToCommand(i);
  }
  return "movement plan executed.";
}

#endif

String checkPlanSchema(JsonArray steps)
{
  for (String step : steps)
  {
    if (step.indexOf("angle") == -1 || step.indexOf("distance") == -1  || step.indexOf("direction") == -1 )
    {
      return "Invalid movement plan schema, it needs to contain angle, distance and direction";
    }
  }
  return "valid";
}
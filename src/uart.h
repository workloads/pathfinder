void jsonCmdReceiveHandler()
{
	int cmdType = jsonCmdReceive["T"].as<int>();
	switch (cmdType) {
		// emergency stop.
		case CMD_EMERGENCY_STOP:
			setGoalSpeed(0, 0);
			break;
			
		case CMD_SPEED_CTRL:
			if (jsonCmdReceive["T"].is<int>() &&
				jsonCmdReceive["L"].is<int>() &&
				jsonCmdReceive["R"].is<int>())
			{
				if (jsonCmdReceive["L"].is<float>() &&
					jsonCmdReceive["R"].is<float>())
				{
					heartbeatStopFlag = false;
					lastCmdRecvTime = millis();
					setGoalSpeed(
						jsonCmdReceive["L"],
						jsonCmdReceive["R"]);
				}
			}
			break;

		case CMD_PWM_INPUT:
			usePIDCompute = false;
			heartbeatStopFlag = false;
			lastCmdRecvTime = millis();
			leftCtrl(jsonCmdReceive["L"]);
			rightCtrl(jsonCmdReceive["R"]);
			break;

		case CMD_ROS_CTRL:
			rosCtrl(
				jsonCmdReceive["X"],
				jsonCmdReceive["Z"]);
			break;

		case CMD_SET_MOTOR_PID:
			setPID(
				jsonCmdReceive["P"],
				jsonCmdReceive["I"],
				jsonCmdReceive["D"],
				jsonCmdReceive["L"]);
			break;

		case CMD_OLED_CTRL:
			oledCtrl(
				jsonCmdReceive["lineNum"],
				jsonCmdReceive["Text"]);
			break;
		case CMD_OLED_DEFAULT:
			setOledDefault();
			break;
			
		case CMD_MODULE_TYPE:
			changeModuleType(
				jsonCmdReceive["cmd"]);
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
				jsonCmdReceive["gx"],
				jsonCmdReceive["gy"],
				jsonCmdReceive["gz"],
				jsonCmdReceive["ax"],
				jsonCmdReceive["ay"],
				jsonCmdReceive["az"],
				jsonCmdReceive["cx"],
				jsonCmdReceive["cy"],
				jsonCmdReceive["cz"]);
			break;

		case CMD_BASE_FEEDBACK:
			baseInfoFeedbackHttp();
			break;
		#if BASE_FEEDBACK_FLOW
		case CMD_BASE_FEEDBACK_FLOW:
			setBaseInfoFeedbackMode(
				jsonCmdReceive["cmd"]);
			break;
		#endif
		case CMD_FEEDBACK_FLOW_INTERVAL:
			setFeedbackFlowInterval(
				jsonCmdReceive["cmd"]);
			break;
			
		case CMD_UART_ECHO_MODE:
			setCmdEcho(
				jsonCmdReceive["cmd"]);
			break;

		case CMD_IMU_SAFETY_STATUS:
			getIMUSafetyStatus();
			break;

		case CMD_LED_CTRL:
			led_pwm_ctrl(
				jsonCmdReceive["IO4"],
				jsonCmdReceive["IO5"]);
			break;

		case CMD_HEART_BEAT_SET:
			changeHeartBeatDelay(
				jsonCmdReceive["cmd"]);
			break;

		case CMD_SET_SPD_RATE:
			setSpdRate(
				jsonCmdReceive["L"],
				jsonCmdReceive["R"]);
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
				jsonCmdReceive["mode"]);
			break;
		case CMD_CONFIG_EOAT:
			configEoAT(
				jsonCmdReceive["pos"],
				jsonCmdReceive["ea"],
				jsonCmdReceive["eb"]);
			break;

		case CMD_SWITCH_OFF:
			switchEmergencyStop();
		break;

		// mission & steps edit & file edit.
		case CMD_SCAN_FILES:
			scanFlashContents();
			break;

		case CMD_CREATE_FILE:
			createFile(
				jsonCmdReceive["name"],
				jsonCmdReceive["content"]);
			break;

		case CMD_READ_FILE:
			readFile(
				jsonCmdReceive["name"]);
			break;

		case CMD_DELETE_FILE:
			deleteFile(
				jsonCmdReceive["name"]);
			break;

		case CMD_APPEND_LINE:
			appendLine(
				jsonCmdReceive["name"],
				jsonCmdReceive["content"]);
			break;

		case CMD_INSERT_LINE:
			insertLine(
				jsonCmdReceive["name"],
				jsonCmdReceive["lineNum"],
				jsonCmdReceive["content"]);
			break;

		case CMD_REPLACE_LINE:
			replaceLine(
				jsonCmdReceive["name"],
				jsonCmdReceive["lineNum"],
				jsonCmdReceive["content"]);
			break;

		case CMD_READ_LINE:
			readSingleLine(
				jsonCmdReceive["name"],
				jsonCmdReceive["lineNum"]);
			break;

		case CMD_DELETE_LINE:
			deleteSingleLine(
				jsonCmdReceive["name"],
				jsonCmdReceive["lineNum"]);
			break;

		case CMD_CREATE_MISSION:
			createMission(
				jsonCmdReceive["name"],
				jsonCmdReceive["intro"]);
			break;

		case CMD_MISSION_CONTENT:
			missionContent(
				jsonCmdReceive["name"]);
			break;

		case CMD_APPEND_STEP_JSON:
			appendStepJson(
				jsonCmdReceive["name"],
				jsonCmdReceive["step"]);
			break;

		case CMD_APPEND_STEP_FB:
			appendStepFB(
				jsonCmdReceive["name"],
				jsonCmdReceive["spd"]);
			break;

		case CMD_APPEND_DELAY:
			appendDelayCmd(
				jsonCmdReceive["name"],
				jsonCmdReceive["delay"]);
			break;

		case CMD_INSERT_STEP_JSON:
			insertStepJson(
				jsonCmdReceive["name"],
				jsonCmdReceive["stepNum"],
				jsonCmdReceive["step"]);
			break;

		case CMD_INSERT_STEP_FB:
			insertStepFB(
				jsonCmdReceive["name"],
				jsonCmdReceive["stepNum"],
				jsonCmdReceive["spd"]);
			break;

		case CMD_INSERT_DELAY:
			insertDelayCmd(
				jsonCmdReceive["name"],
				jsonCmdReceive["stepNum"],
				jsonCmdReceive["spd"]);
			break;

		case CMD_REPLACE_STEP_JSON:
			replaceStepJson(
				jsonCmdReceive["name"],
				jsonCmdReceive["stepNum"],
				jsonCmdReceive["step"]);
			break;

		case CMD_REPLACE_STEP_FB:
			replaceStepFB(
				jsonCmdReceive["name"],
				jsonCmdReceive["stepNum"],
				jsonCmdReceive["spd"]);
			break;

		case CMD_REPLACE_DELAY:
			replaceDelayCmd(
				jsonCmdReceive["name"],
				jsonCmdReceive["stepNum"],
				jsonCmdReceive["delay"]);
			break;

		case CMD_DELETE_STEP:
			deleteStep(
				jsonCmdReceive["name"],
				jsonCmdReceive["stepNum"]);
			break;

		case CMD_MOVE_TO_STEP:
			moveToStep(
				jsonCmdReceive["name"],
				jsonCmdReceive["stepNum"]);
			break;

		case CMD_MISSION_PLAY:
			missionPlay(
				jsonCmdReceive["name"],
				jsonCmdReceive["times"]);
			break;

		// wifi settings.
		case CMD_WIFI_ON_BOOT:
			configWifiModeOnBoot(
				jsonCmdReceive["cmd"]);
			break;
			
		case CMD_SET_AP:
			wifiModeAP(
				jsonCmdReceive["ssid"],
				jsonCmdReceive["password"]);
			break;

		case CMD_SET_STA:
			wifiModeSTA(
				jsonCmdReceive["ssid"],
				jsonCmdReceive["password"]);
			break;

		case CMD_WIFI_APSTA:
			wifiModeAPSTA(
				jsonCmdReceive["ap_ssid"],
				jsonCmdReceive["ap_password"],
				jsonCmdReceive["sta_ssid"],
				jsonCmdReceive["sta_password"]);
			break;
				
		case CMD_WIFI_INFO:
			wifiStatusFeedback();
			break;

		case CMD_WIFI_CONFIG_CREATE_BY_STATUS:
			createWifiConfigFileByStatus();
			break;

		case CMD_WIFI_CONFIG_CREATE_BY_INPUT:
			createWifiConfigFileByInput(
				jsonCmdReceive["mode"],
				jsonCmdReceive["ap_ssid"],
				jsonCmdReceive["ap_password"],
				jsonCmdReceive["sta_ssid"],
				jsonCmdReceive["sta_password"]);
			break;

		case CMD_WIFI_STOP:
			wifiStop();
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
					jsonCmdReceive["cmd"]);
				break;
		#endif

		// mainType & moduleType settings.
		case CMD_MM_TYPE_SET:
			mm_settings(
				jsonCmdReceive["main"],
				jsonCmdReceive["module"]);
			saveMainTypeModuleTpye(
				jsonCmdReceive["main"],
				jsonCmdReceive["module"]);
			break;
		}
	}

void serialCtrl()
{
	static String receivedData;

	while (Serial.available() > 0)
	{
		char receivedChar = Serial.read();
		receivedData += receivedChar;

		// Detect the end of the JSON string based on a specific termination character
		if (receivedChar == '\n')
		{
			// Now we have received the complete JSON string
			DeserializationError err = deserializeJson(jsonCmdReceive, receivedData);
			if (err == DeserializationError::Ok)
			{
				#if INFO_PRINT
					if (InfoPrint == 1 && uartCmdEcho)
					{
						Serial.print(receivedData);
					}
				#endif
				jsonCmdReceiveHandler();
			}
			else
			{
				// Handle JSON parsing error here
			}
			// Reset the receivedData for the next JSON string
			receivedData = "";
		}
	}
}
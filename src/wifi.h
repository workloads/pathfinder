// wifi ctrl functions.
// you can refer to this website below to upload a config file to ESP32 Flash.
// https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/

// libraries:
// #include <LittleFS.h>
// #include <WIFI.h>
// #include <ArduinoJson.h>

#include <ArduinoUniqueID.h>

// you need to init Serial.
// bool InfoPrint = true;

// wifi config
// wifi mode on boot.
// 0: OFF (you need to use uart-command or upload a new wifiConfig.json to turn it on again)
// 1: AP (default mode as a brand new product)
// 2: STA
// 3: AP+STA (default mode after first wifi connection succeed)
byte WIFI_MODE_ON_BOOT = 3;

const char* sta_ssid = "workloads";
const char* sta_password = "workloads";

const char* ap_ssid = "pathfinder";
const char* ap_password = "pathfinder";

String hostname;

// true: change the WIFI_MODE_ON_BOOT to 3 when first STA mode succeed.
bool defaultModeToAPSTA = true;

// wifiConfig.yaml example:
// wifi_mode_on_boot:3
// sta_ssid:"WIFI_NAME"
// sta_ssid:"WIFI_PASSWORD"
// ap_ssid:"WIFI_NAME"
// ap_ssid:"WIFI_PASSWORD"
File wifiConfigYaml;


// other args:
unsigned long connectionStartTime;
unsigned long connectionTimeout = 15000;
byte WIFI_CURRENT_MODE = -1;
IPAddress localIP;
JsonDocument wifiDoc;
bool wifiConfigFound = false;

// Auto-reconnection variables
unsigned long lastWifiCheck = 0;
unsigned long wifiCheckInterval = 30000; // Check WiFi status every 30 seconds
bool autoReconnectEnabled = true;
int reconnectAttempts = 0;
int maxReconnectAttempts = 5;


// update oled accroding to wifi settings.
void updateOledWifiInfo() {
  wl_status_t wifiStatus = WiFi.status();
  String statusIndicator = "";
  
  // Add connection status indicator
  if (wifiStatus == WL_CONNECTED) {
    statusIndicator = " ✓";
  } else if (reconnectAttempts > 0) {
    statusIndicator = " ⚠";
  } else {
    statusIndicator = " ✗";
  }
  
  switch(WIFI_CURRENT_MODE) {
  case 0: 
    screenLine_0 = "AP: OFF";
    screenLine_1 = "ST: OFF";
    break;
  case 1:
    screenLine_0 = String("AP:") + ap_ssid;
    screenLine_1 = String("AP:") + localIP.toString();
    screenLine_2 = "ST: OFF";
    break;
  case 2:
    screenLine_0 = "AP: OFF";
    screenLine_1 = String("ST:") + localIP.toString() + statusIndicator;
    if (wifiStatus == WL_CONNECTED) {
      screenLine_2 = String("RSSI: ") + WiFi.RSSI() + "dBm";
    } else {
      screenLine_2 = "Disconnected";
    }
    break;
  case 3:
    screenLine_0 = String("AP:") + ap_ssid;
    screenLine_1 = String("ST:") + localIP.toString() + statusIndicator;
    if (wifiStatus == WL_CONNECTED) {
      screenLine_2 = String("RSSI: ") + WiFi.RSSI() + "dBm";
    } else {
      screenLine_2 = "Disconnected";
    }
    break;
  }
  oled_update();
}


// load the wifiConfig.json form Flash.
// the file name is wifiConfig.json in root path.
bool loadWifiConfig() {
	wifiConfigYaml = LittleFS.open("/wifiConfig.json", "r");
	if (wifiConfigYaml) {
		if (InfoPrint == 1) {Serial.println("/wifiConfig.json load succeed.");}
		
		String line = wifiConfigYaml.readStringUntil('\n');

		// parse the YAML file using ArduinoJson.
		deserializeJson(wifiDoc, line);

		// read configuration values.
		WIFI_MODE_ON_BOOT = wifiDoc["wifi_mode_on_boot"];
		sta_ssid = wifiDoc["sta_ssid"];
		sta_password = wifiDoc["sta_password"];
		ap_ssid = wifiDoc["ap_ssid"];
		ap_password = wifiDoc["ap_password"];

		if (InfoPrint == 1) {
			Serial.println(line);
		}

		wifiConfigYaml.close();
		wifiConfigFound = true;
		jsonInfoHttp.clear();
  	jsonInfoHttp["ip"] = "/wifiConfig.json load succeed.";
 		jsonInfoHttp["wifi_mode_on_boot"] = WIFI_MODE_ON_BOOT;
 		jsonInfoHttp["sta_ssid"] = sta_ssid;
 		jsonInfoHttp["sta_password"] = sta_password;
 		jsonInfoHttp["ap_ssid"] = ap_ssid;
 		jsonInfoHttp["ap_password"] = ap_password;
		return true;

	} else {
		if (InfoPrint == 1) {Serial.println("cound not found wifiConfig.json.");}
		wifiConfigFound = false;
		return false;
	}
}


// get the ip address.
IPAddress getIPAddress(byte inputMode) {
	localIP = WiFi.localIP();
	if (InfoPrint == 1) {
		Serial.print("IP: ");
		Serial.println(localIP.toString());
	}

	jsonInfoHttp.clear();
  jsonInfoHttp["ip"] = localIP.toString();
	return localIP;
}


// create a wifiConfig.json file
// from the args already be using.
bool createWifiConfigFileByStatus() {
	if (WIFI_MODE_ON_BOOT != 0){
		wifiDoc.clear();
		wifiDoc["wifi_mode_on_boot"] = WIFI_MODE_ON_BOOT;
		wifiDoc["sta_ssid"] = sta_ssid;
		wifiDoc["sta_password"] = sta_password;
		wifiDoc["ap_ssid"] = ap_ssid;
		wifiDoc["ap_password"] = ap_password;

		File configFile = LittleFS.open("/wifiConfig.json", "w");
		if (configFile) {
			serializeJson(wifiDoc, configFile);
			configFile.close();
			if (InfoPrint == 1) {
				Serial.println("/wifiConfig.json created.");
			}
			jsonInfoHttp.clear();
  		jsonInfoHttp["info"] = "/wifiConfig.json created.";
			jsonInfoHttp["wifi_mode_on_boot"] = WIFI_MODE_ON_BOOT;
			jsonInfoHttp["sta_ssid"] = sta_ssid;
			jsonInfoHttp["sta_password"] = sta_password;
			jsonInfoHttp["ap_ssid"] = ap_ssid;
			jsonInfoHttp["ap_password"] = ap_password;
			return true;
		} else {
			jsonInfoHttp.clear();
  		jsonInfoHttp["info"] = "/wifiConfig.json open failed.";
			return false;
		}
	} else {
		jsonInfoHttp.clear();
  	jsonInfoHttp["info"] = "not for this wifi_mode_on_boot.";
		return false;
	}
}


// set wifi as AP mode.
bool wifiModeAP(const char* input_ssid, const char* input_password) {
	WiFi.disconnect();
	if (InfoPrint == 1) {Serial.println("wifi mode on boot: AP");}
	// WiFi.mode(WIFI_AP);
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(input_ssid, input_password);
	if (InfoPrint == 1) {
		Serial.println("AP mode starts...");
		Serial.print("SSID: ");
		Serial.println(input_ssid);
		Serial.print("Password: ");
		Serial.println(input_password);
		Serial.println("AP Address: 192.168.4.1");
	}
	WIFI_CURRENT_MODE = 1;
	localIP = WiFi.localIP();
	ap_ssid = input_ssid;
	ap_password = input_password;

	updateOledWifiInfo();

	jsonInfoHttp.clear();
  jsonInfoHttp["info"] = "AP mode starts";
  jsonInfoHttp["ap_ssid"] = ap_ssid;
  jsonInfoHttp["ap_password"] = ap_password;

	return true;
}


// set wifi as STA mode.
bool wifiModeSTA(const char* input_ssid, const char* input_password) {
	WiFi.disconnect();
	if (InfoPrint == 1) {Serial.println("wifi mode on boot: STA");}
	// WiFi.mode(WIFI_STA);
	WiFi.mode(WIFI_AP_STA);
	WiFi.begin(input_ssid, input_password);
	connectionStartTime = millis();

	if (InfoPrint == 1) {Serial.println("STA mode starts: connecting to ");
					Serial.println(input_ssid);}
	while (WiFi.status() != WL_CONNECTED) {
		unsigned long currentTime = millis();
		if (InfoPrint == 1) {Serial.print(".");}
		delay(500);

		if (currentTime - connectionStartTime >= connectionTimeout) {
			WIFI_CURRENT_MODE = -1;
			if (InfoPrint == 1) {Serial.println(".");Serial.println("STA connection timeout.");}
			wifiModeAP(ap_ssid, ap_password);
			updateOledWifiInfo();

			jsonInfoHttp.clear();
			jsonInfoHttp["info"] = "STA connection timeout.";

			return false;
			break;
		}
	}

	if (InfoPrint == 1) {Serial.println(".");Serial.println("STA connection succeed.");}
	WIFI_CURRENT_MODE = 2;
	getIPAddress(WIFI_CURRENT_MODE);
	sta_ssid = input_ssid;
	sta_password = input_password;

	jsonInfoHttp.clear();
	jsonInfoHttp["info"] = "STA connection succeed.";
	jsonInfoHttp["wifi_mode_on_boot"] = WIFI_MODE_ON_BOOT;
	jsonInfoHttp["sta_ssid"] = sta_ssid;
	jsonInfoHttp["sta_password"] = sta_password;
	jsonInfoHttp["ap_ssid"] = ap_ssid;
	jsonInfoHttp["ap_password"] = ap_password;

	if (defaultModeToAPSTA && !wifiConfigFound) {
		WIFI_MODE_ON_BOOT = 3;
		if (InfoPrint == 1) {Serial.println("[default] wifi mode on boot: AP+STA");}
		jsonInfoHttp["info"] = "[default] wifi mode on boot: AP+STA";
		createWifiConfigFileByStatus();
	}
	updateOledWifiInfo();

	return true;
}


// set wifi as AP+STA mode.
bool wifiModeAPSTA(const char* input_ap_ssid, const char* input_ap_password, const char* input_sta_ssid, const char* input_sta_password) {
	WiFi.disconnect();
	if (InfoPrint == 1) {Serial.println("wifi mode on boot: AP+STA");}
	WiFi.mode(WIFI_AP_STA);
	WiFi.softAP(input_ap_ssid, input_ap_password);
	WiFi.setHostname(hostname.c_str());
	if (InfoPrint == 1) {
		Serial.println("AP/AP+STA mode starts...");
		Serial.print("AP SSID: ");
		Serial.println(input_ap_ssid);
		Serial.print("AP Password: ");
		Serial.println(input_ap_password);
		Serial.println("AP Address: 192.168.4.1");
	}
	ap_ssid = input_ap_ssid;
	ap_password = input_ap_password;
	
	WiFi.begin(input_sta_ssid, input_sta_password);
	connectionStartTime = millis();

	if (InfoPrint == 1) {Serial.print("STA/AP+STA mode starts: connecting to ");
					Serial.println(input_sta_ssid);}
	while (WiFi.status() != WL_CONNECTED) {
		unsigned long currentTime = millis();
		if (InfoPrint == 1) {Serial.print(".");}
		delay(500);

		if (currentTime - connectionStartTime >= connectionTimeout) {
			WIFI_CURRENT_MODE = -1;
			if (InfoPrint == 1) {Serial.println(".");Serial.println("STA connection timeout.");}
			wifiModeAP(ap_ssid, ap_password);
			updateOledWifiInfo();

			jsonInfoHttp.clear();
			jsonInfoHttp["info"] = "STA connection timeout.";

			return false;
			break;
		}
	}

	if (InfoPrint == 1) {Serial.println("STA connection succeed.");}
	WIFI_CURRENT_MODE = 3;
	getIPAddress(WIFI_CURRENT_MODE);
	sta_ssid = input_sta_ssid;
	sta_password = input_sta_password;
	if (defaultModeToAPSTA && !wifiConfigFound) {
		WIFI_MODE_ON_BOOT = 3;
		if (InfoPrint == 1) {Serial.println("[default] wifi mode on boot: AP+STA");}
		createWifiConfigFileByStatus();
	}
	updateOledWifiInfo();

	jsonInfoHttp.clear();
	jsonInfoHttp["info"] = "STA connection succeed.";
	jsonInfoHttp["wifi_mode_on_boot"] = WIFI_MODE_ON_BOOT;
	jsonInfoHttp["sta_ssid"] = sta_ssid;
	jsonInfoHttp["sta_password"] = sta_password;
	jsonInfoHttp["ap_ssid"] = ap_ssid;
	jsonInfoHttp["ap_password"] = ap_password;
	
	return true;
}


// disconnect wifi.
void wifiStop() {
	WiFi.disconnect();
	WIFI_CURRENT_MODE = 0;
	WiFi.mode(WIFI_AP_STA);
	updateOledWifiInfo();
}


// wifi mode on boot starts.
bool wifiModeOnBoot() {
	bool funcStatus = false;
	switch(WIFI_MODE_ON_BOOT) {
	case 0: 
		if (InfoPrint == 1) {
			Serial.println("wifi mode on boot: OFF");
		}
		funcStatus = true;
		WIFI_CURRENT_MODE = 0;
		WiFi.mode(WIFI_AP_STA);
		break;
	case 1:
		funcStatus = wifiModeAP(ap_ssid, ap_password);
		break;
	case 2:
		funcStatus = wifiModeSTA(sta_ssid, sta_password);
		break;
	case 3:
		funcStatus = wifiModeAPSTA(ap_ssid, ap_password, sta_ssid, sta_password);
		break;
	}
	return funcStatus;
}


// change the WIFI_MODE_ON_BOOT.
void configWifiModeOnBoot(byte inputMode) {
	WIFI_MODE_ON_BOOT = inputMode;
	if (InfoPrint == 1) {
		Serial.print("wifi_mode_on_boot: ");
		Serial.println(WIFI_MODE_ON_BOOT);
	}
	createWifiConfigFileByStatus();
}


// create a wifiConfig.json file
// from the args input.
void createWifiConfigFileByInput(byte inputMode, const char* inputApSsid, const char* inputApPassword, const char* inputStaSsid, const char* inputStaPassword) {
	WIFI_MODE_ON_BOOT = inputMode;
	wifiModeAPSTA(inputApSsid, inputApPassword, inputStaSsid, inputStaPassword);
	if (InfoPrint == 1) {
		Serial.print("wifi_mode_on_boot: ");
		Serial.println(WIFI_MODE_ON_BOOT);
	}
	createWifiConfigFileByStatus();
}


// wifi information feedback.
String wifiStatusFeedback() {
	wifiDoc["ip"] = localIP.toString();
	wifiDoc["rssi"] = WiFi.RSSI();

	serializeJson(wifiDoc, Serial);

	jsonInfoHttp.clear();
	jsonInfoHttp["ip"] = wifiDoc["ip"];
	jsonInfoHttp["rssi"] = wifiDoc["rssi"];
	jsonInfoHttp["wifi_mode_on_boot"] = WIFI_MODE_ON_BOOT;
	jsonInfoHttp["sta_ssid"] = sta_ssid;
	jsonInfoHttp["sta_password"] = sta_password;
	jsonInfoHttp["ap_ssid"] = ap_ssid;
	jsonInfoHttp["ap_password"] = ap_password;
	jsonInfoHttp["mac"] = thisMacStr;

    // Clear previous API response, see https://arduinojson.org/v6/api/jsondocument/clear/
	apiResponse.clear();

    // Populate current API response
	apiResponse[0]["encrypted"] = true;
	apiResponse[0]["rssi"] = WiFi.RSSI();
	apiResponse[0]["ssid"] = sta_ssid;

	// Create an empty string to serialize the JSON response to
	String jsonResponse = "";
	serializeJsonPretty(apiResponse, jsonResponse);
	return(jsonResponse);
}

// device information feedback.
String deviceStatusFeedback() {

	// Clear previous API response, see https://arduinojson.org/v6/api/jsondocument/clear/
	apiResponse.clear();

	// Populate current API response
	apiResponse[0]["features"]["isHttpEnabled"] = true;
	apiResponse[0]["features"]["isInsightsEnabled"] = false;

	apiResponse[0]["identifiers"]["long"]  = "pathfinder-a1b2c3d4e5f6g7h8";
	apiResponse[0]["identifiers"]["short"] = "pathfinder-g7h8";

	apiResponse[0]["name"] = "Pathfinder";
	apiResponse[0]["uptime"] = 1234;

	apiResponse[0]["versions"]["api"]  = "0.1.0";
	apiResponse[0]["versions"]["app"] = "0.10.0";

	// Create an empty string to serialize the JSON response to
	String jsonResponse = "";
	serializeJsonPretty(apiResponse, jsonResponse);
	return(jsonResponse);
}


// initialize hostname from unique ID
void initHostname() {
	char idString[17]; // Buffer to hold the hexadecimal string (16 characters + null terminator)
	for (int i = 0; i < 8; i++) {
		sprintf(&idString[i * 2], "%02X", UniqueID8[i]);
	}
	idString[16] = '\0'; // Ensure null termination
	hostname = String("pathfinder-") + String(idString);
}

// Check WiFi status and attempt reconnection if needed
void checkWifiAndReconnect() {
	if (!autoReconnectEnabled) return;
	
	unsigned long currentTime = millis();
	
	// Check if it's time to check WiFi status
	if (currentTime - lastWifiCheck >= wifiCheckInterval) {
		lastWifiCheck = currentTime;
		
		// Check current WiFi status
		wl_status_t wifiStatus = WiFi.status();
		
		if (wifiStatus != WL_CONNECTED) {
			if (InfoPrint == 1) {
				Serial.print("WiFi disconnected. Status: ");
				Serial.println(wifiStatus);
			}
			
			// Only attempt reconnection if we have valid credentials and haven't exceeded max attempts
			if (strlen(sta_ssid) > 0 && strlen(sta_password) > 0 && reconnectAttempts < maxReconnectAttempts) {
				reconnectAttempts++;
				
				if (InfoPrint == 1) {
					Serial.print("Attempting WiFi reconnection (attempt ");
					Serial.print(reconnectAttempts);
					Serial.print("/");
					Serial.print(maxReconnectAttempts);
					Serial.println(")");
				}
				
				// Attempt to reconnect based on current mode
				bool reconnectSuccess = false;
				switch(WIFI_MODE_ON_BOOT) {
					case 2: // STA mode
						reconnectSuccess = wifiModeSTA(sta_ssid, sta_password);
						break;
					case 3: // AP+STA mode
						reconnectSuccess = wifiModeAPSTA(ap_ssid, ap_password, sta_ssid, sta_password);
						break;
					default:
						// For other modes, try STA connection
						reconnectSuccess = wifiModeSTA(sta_ssid, sta_password);
						break;
				}
				
				if (reconnectSuccess) {
					reconnectAttempts = 0; // Reset counter on successful connection
					if (InfoPrint == 1) {
						Serial.println("WiFi reconnection successful!");
					}
					updateOledWifiInfo(); // Update display on successful reconnection
				} else {
					if (InfoPrint == 1) {
						Serial.println("WiFi reconnection failed.");
					}
					updateOledWifiInfo(); // Update display on failed reconnection
				}
			} else {
				if (InfoPrint == 1) {
					Serial.println("WiFi reconnection disabled or max attempts reached.");
				}
			}
		} else {
			// WiFi is connected, reset reconnect attempts counter
			if (reconnectAttempts > 0) {
				reconnectAttempts = 0;
				if (InfoPrint == 1) {
					Serial.println("WiFi connection restored.");
				}
			}
		}
	}
}

// Enable/disable auto-reconnection
void setAutoReconnect(bool enabled) {
	autoReconnectEnabled = enabled;
	if (InfoPrint == 1) {
		Serial.print("WiFi auto-reconnection ");
		Serial.println(enabled ? "enabled" : "disabled");
	}
}

// Set WiFi check interval
void setWifiCheckInterval(unsigned long interval) {
	wifiCheckInterval = interval;
	if (InfoPrint == 1) {
		Serial.print("WiFi check interval set to ");
		Serial.print(interval / 1000);
		Serial.println(" seconds");
	}
}

// Get current WiFi connection status
bool isWifiConnected() {
	return WiFi.status() == WL_CONNECTED;
}

// Get WiFi signal strength
int getWifiRSSI() {
	return WiFi.RSSI();
}

// Get WiFi connection info as string
String getWifiStatusString() {
	wl_status_t status = WiFi.status();
	String statusText = "";
	
	switch(status) {
		case WL_CONNECTED:
			statusText = "Connected";
			break;
		case WL_NO_SSID_AVAIL:
			statusText = "No SSID Available";
			break;
		case WL_CONNECT_FAILED:
			statusText = "Connection Failed";
			break;
		case WL_CONNECTION_LOST:
			statusText = "Connection Lost";
			break;
		case WL_DISCONNECTED:
			statusText = "Disconnected";
			break;
		default:
			statusText = "Unknown Status";
			break;
	}
	
	return statusText;
}

// wifi init.
void initWifi() {
	initHostname();
	loadWifiConfig();
	wifiModeOnBoot();
}
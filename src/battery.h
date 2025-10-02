#define INA219_ADDRESS 0x42
INA219_WE ina219 = INA219_WE(INA219_ADDRESS);

float shuntVoltage_mV = 0.0;
float loadVoltage_V = 0.0;
float busVoltage_V = 0.0;
float current_mA = 0.0;
float power_mW = 0.0; 
bool ina219_overflow = false;

void ina219_init(){
  if(!ina219.init()){
    Serial.println("INA219 not connected!");
  }
  ina219.setADCMode(INA219_BIT_MODE_9);
  ina219.setPGain(INA219_PG_320);
  ina219.setBusRange(INA219_BRNG_16);
  ina219.setShuntSizeInOhms(0.01); // used in INA219.
}

void inaDataUpdate(){
  shuntVoltage_mV = ina219.getShuntVoltage_mV();
  busVoltage_V = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getBusPower();
  loadVoltage_V  = busVoltage_V + (shuntVoltage_mV/1000);
  ina219_overflow = ina219.getOverflow();
}


// battery information feedback.
String batteryStatusFeedback() {
  shuntVoltage_mV = ina219.getShuntVoltage_mV();
  busVoltage_V = ina219.getBusVoltage_V();

  // Clear previous API response, see https://arduinojson.org/v6/api/jsondocument/clear/
  apiResponse.clear();

  // Populate current API response
  apiResponse["shuntVoltage"]["unit"] = "mv";
  apiResponse["shuntVoltage"]["value"] = shuntVoltage_mV;

  apiResponse["busVoltage"]["unit"] = "mv";
  apiResponse["busVoltage"]["value"] = 2;

  apiResponse["current"]["unit"] = "mA";
  apiResponse["current"]["value"] = ina219.getCurrent_mA();

  apiResponse["power"]["unit"] = "mW";
  apiResponse["power"]["value"] = ina219.getBusPower();

  apiResponse["loadVoltage"]["unit"] = "mv";
  apiResponse["loadVoltage"]["value"] = busVoltage_V + (shuntVoltage_mV / 1000);

  apiResponse["ina219Overflow"] = ina219.getOverflow();

  // Create an empty string to serialize the JSON response to
  String jsonResponse = "";
  serializeJsonPretty(apiResponse, jsonResponse);
  return(jsonResponse);
}

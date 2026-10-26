#if API_V2
#include "modles.h"
#include <vector>

const unsigned long AUTO_STOP_DELAY = 3000;
unsigned long lastCommandTime = 0;

std::vector<step> movementSteps;


void move(int angle, int distance, int direction)
{
  // Constants
  const float speed = 0.5;
  const int spinDelay = 0;       // 2 seconds delay before spinning
  const int msPerDegree = 800 / 90; // ms needed for 1 degree of rotation
  //move amount
  // float L;
  // float R;

  struct vector movement;
  

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
      movement.l = speed;
      movement.r = speed * -1;
      rotationDuration = angle * msPerDegree;
    }
    else if (angle > 180 && angle < 360)
    {
      // Spin left
      movement.l = speed * -1;
      movement.r = speed;
      rotationDuration = (360 - angle) * msPerDegree;
    }
    else
    {
      Serial.println("Invalid angle value");
      return;
    }

    // Perform rotation
    Serial.println("Rotation command: " + toString(&movement));
    Serial.println("Rotation duration: " + String(rotationDuration) + " ms");
    setAPIGoalSpeed(&movement);
    delay(rotationDuration);

    // Stop rotation
    Serial.println("Stopping rotation");
    setAPIGoalSpeed(0, 0);
    delay(100); // Short pause after rotation
  }

  // Step 2: Move in the specified direction for the given distance
  movement.l = speed * direction;
  movement.r = speed * direction;

  // Perform movement
  Serial.println("Movement command: " + toString(&movement));
  setAPIGoalSpeed(&movement);

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
      setAPIGoalSpeed(&movement);
      lastCommandTime = millis();
    }
    delay(10); // Small delay to prevent excessive looping
  }

  // Stop movement
  Serial.println("Stopping movement");
  movement.l = 0;
  movement.r = 0;
  setAPIGoalSpeed(&movement);
  lastCommandTime = millis();
}

void planStepToCommandV2(step movementPlanStep)
{

  Serial.println("angle: " + String(movementPlanStep.angle));
  Serial.println("Distance: " + String(movementPlanStep.distance));
  Serial.println("Direction: " + movementPlanStep.direction);
  if (movementPlanStep.direction == "forward")
  {
    move(movementPlanStep.angle, movementPlanStep.distance, 1);
  }
  else if (movementPlanStep.direction == "backward")
  {
    move(movementPlanStep.angle, movementPlanStep.distance, -1);
  }
  else
  {
    Serial.println("Invalid direction");
    return;
  }
}

String getSteps(){
  JsonDocument doc;
  String output;
  // for (step s : movementSteps)
  for (int i = 0; i < movementSteps.size(); i++)
  {
    doc[i]["angle"] = movementSteps[i].angle;
    doc[i]["distance"] = movementSteps[i].distance;
    doc[i]["direction"] = movementSteps[i].direction;
  }
  serializeJson(doc, output);
  Serial.println(output);
  
  return output;

}

String runSteps(){
  String output = "run steps\n";
  for (step s : movementSteps)
  // for (int i = 0; i < arraySize; i++)
  {
    planStepToCommandV2(s);
    output = output + "run command: angle" + s.angle + " distance: " + s.distance + "direction: " + s.direction + "\n";
    
  }
  
  return output;
}

String apiv2MovementPlanHandler(String jsonInput)
{
  JsonDocument input;

  // parse the JSON file using ArduinoJson.
  deserializeJson(input, jsonInput);
  Serial.println("Received a movement plan"); //, String(input["name"]));
                                              //    Serial.println(input);
  // for (int i = 0; i < input["steps"].size(); i++) {
  JsonArray steps = input["steps"].as<JsonArray>();
  String checkResult = checkPlanSchema(steps);
  movementSteps.clear();
  step moveStep;
  if (checkResult != "valid")
  {
    return checkResult;
  }
  for (String i : steps)
  {
    Serial.println("Step");
    JsonDocument step;
    deserializeJson(step, i);

    moveStep.angle = step["angle"].as<int>();
    moveStep.distance = step["distance"].as<int>();
    moveStep.direction = step["direction"].as<String>();
    // planStepToCommandV2(moveSteps[count]);
    movementSteps.push_back(moveStep);
    
  }
  // runSteps();
  return "movement plan executed.";
}
#endif
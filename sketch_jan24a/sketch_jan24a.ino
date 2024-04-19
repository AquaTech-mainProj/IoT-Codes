// Include the necessary libraries
#include <Arduino.h>

// Define the pin for the IR sensor
const int irSensorPin = 4;

void setup() {
  Serial.begin(9600);
  
  // Initialize the IR sensor pin
  pinMode(irSensorPin, INPUT);
}

void loop() {
  // Read the state of the IR sensor
  int irSensorValue = digitalRead(irSensorPin);

  // Print the sensor value to the Serial Monitor
  Serial.print("IR Sensor Value: ");
  Serial.println(irSensorValue);

  // Add a short delay for stability
  delay(2000);
}


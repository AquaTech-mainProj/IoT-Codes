#include <Arduino.h>

// Define the analog pin connected to the moisture sensor
const int moistureSensorPin = A0;

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 baud rate
}

void loop() {
  // Read the moisture sensor value
  int moistureValue = analogRead(moistureSensorPin);

  // Print the moisture value to the Serial Monitor
  Serial.print("Moisture value: ");
  Serial.println(moistureValue);

  delay(1000); // Delay for 1 second
}

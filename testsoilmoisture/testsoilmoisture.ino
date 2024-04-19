// Analog pin for soil moisture sensor
const int soilMoisturePin = 15; // Connect to pin D15 better to connect to D34

// Define the range of the sensor readings
// const int dryValue = 4095;   // Maximum sensor value when the soil is dry
//const int wetValue = 1401;       // Minimum sensor value when the soil is wet

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
}

void loop() {
  // Read soil moisture sensor value
  int soilMoistureValue = analogRead(soilMoisturePin);
  // Serial.print("Raw: ");
  // Serial.println(soilMoistureValue);

  // Map the raw sensor value to a percentage
  int moisturePercentage = map(soilMoistureValue, 1401, 4095, 100, 0);

  // Ensure the moisture percentage stays within the valid range of 0 to 100
  moisturePercentage = constrain(moisturePercentage, 0, 100);

  // Print moisture percentage
  Serial.print("Soil Moisture Percentage: ");
  Serial.println(moisturePercentage);

  delay(2000); // Delay for readability
}

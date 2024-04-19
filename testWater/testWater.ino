#define pumpPin 4          // Relay control pin (connected to D4)
#define moisturePin 15     // Soil moisture sensor analog output pin (connected to D15)
#define wetValue 1401      // Maximum sensor reading when soil is wet
#define dryValue 4085      // Minimum sensor reading when soil is dry
#define thresholdValue 50  // Moisture threshold percentage for watering

void setup() {
  Serial.begin(9600);
  pinMode(pumpPin, OUTPUT);
}

void loop() {
  int moistureValue = analogRead(moisturePin); // Read moisture sensor value
  float moisturePercentage = map(moistureValue, wetValue, dryValue, 100, 0); // Map raw value to percentage
  moisturePercentage = constrain(moisturePercentage, 0, 100);
  Serial.print("Moisture Percentage: ");
  Serial.println(moisturePercentage);
 
  if (moisturePercentage < thresholdValue) {
    digitalWrite(pumpPin, HIGH); // Turn on pump
    Serial.println("Pump ON");
  } else {
    digitalWrite(pumpPin, LOW); // Turn off pump
    Serial.println("Pump OFF");
  }

  // Delay before next moisture check
  delay(2000); 
}

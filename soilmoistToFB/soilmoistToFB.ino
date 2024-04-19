#include <Arduino.h>
#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// Insert your network credentials
#define WIFI_SSID "BB's S10 Lite"
#define WIFI_PASSWORD "password1234"

// Insert Firebase project API Key
#define API_KEY "AIzaSyD9odwLVX8f6W3A8_zBn0hXrMrqgntb1EE"

// Insert RTDB URL
#define DATABASE_URL "https://aquatech01234-default-rtdb.asia-southeast1.firebasedatabase.app/"

// Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Analog pin for soil moisture sensor
const int soilMoisturePin = 34; // Connect to pin D34

void setup() {
  Serial.begin(115200);
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  
  // Initialize Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = "leonjosemathew718@gmail.com"; // Change this to your Firebase authentication email
  auth.user.password = "123456@"; // Change this to your Firebase authentication password
  Firebase.begin(&config, &auth);

  if (Firebase.ready()) {
    Serial.println("Connected to Firebase");
  } else {
    Serial.println("Failed to connect to Firebase");
    Serial.println("Reason: " + String(fbdo.errorReason()));
    while (1);
  }
}

void loop() {
  // Read soil moisture sensor value
  int soilMoistureValue = analogRead(soilMoisturePin);
  Serial.print("Raw: ");
  Serial.println(soilMoistureValue);
  // Map the raw sensor value to a percentage
  int moisturePercentage = map(soilMoistureValue, 1401, 4095, 100, 0);
  
  // Ensure the moisture percentage stays within the valid range of 0 to 100
  moisturePercentage = constrain(moisturePercentage, 0, 100);

  // Print moisture percentage
  Serial.print("Soil Moisture Percentage: ");
  Serial.println(moisturePercentage);
  
  // Store moisture percentage in Firebase
  if (Firebase.RTDB.setInt(&fbdo, "SoilMoisture/Percentage", moisturePercentage)) {
    Serial.println("Data sent to Firebase");
  } else {
    Serial.println("Failed to send data to Firebase");
    Serial.println("Reason: " + String(fbdo.errorReason()));
  }

  delay(2000); // Delay for readability
}

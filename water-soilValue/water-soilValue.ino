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

// Pin for LED
const int ledPin = 2; // Change this to your LED pin

// Function to calculate the remaining water amount
float calculateRemainingWater(float waterAmount, int soilMoisturePercentage) {
  float remainingWater = waterAmount - (waterAmount * soilMoisturePercentage / 100.0);
  return remainingWater;
}

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
  
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Read soil moisture sensor value
  int soilMoistureValue = analogRead(soilMoisturePin);
  //Serial.print("Soil Moisture Raw Value: ");
  //Serial.println(soilMoistureValue);
  
  // Map the raw sensor value to a percentage
  int soilMoisturePercentage = map(soilMoistureValue, 1401, 4095, 100, 0);
  
  // Ensure the moisture percentage stays within the valid range of 0 to 100
  soilMoisturePercentage = constrain(soilMoisturePercentage, 0, 100);
  
  Serial.print("Soil Moisture Percentage: ");
  Serial.println(soilMoisturePercentage);
  
  // Fetch water amount value from Firebase
  if (Firebase.RTDB.getInt(&fbdo, "water/amount")) {
    Serial.println("Fetching water amount value...");
    while (fbdo.dataType() != "int") {
      delay(100);
    }
    int waterAmount = fbdo.intData();
    Serial.print("Water amount from Firebase: ");
    Serial.print(waterAmount);
    Serial.println(" ml");

    // Calculate remaining water amount after considering soil moisture
    float remainingWater = calculateRemainingWater(waterAmount, soilMoisturePercentage);
    Serial.print("Remaining Water after soil moisture adjustment: ");
    Serial.print(remainingWater);
    Serial.println(" ml");

    // Display water amount using LED
    if (remainingWater > 0) {
      digitalWrite(ledPin, HIGH); // Turn on LED
    } else {
      digitalWrite(ledPin, LOW); // Turn off LED
    }
  } else {
    Serial.println("Failed to fetch water amount value");
    Serial.println("Reason: " + String(fbdo.errorReason()));
  }

  delay(5000); // Adjust delay as needed
}
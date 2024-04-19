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

// Pin for LED
const int ledPin = 2; // Change this to your LED pin

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
  // Fetch water value from Firebase
  if (Firebase.RTDB.getInt(&fbdo, "water/amount")) {
    Serial.println("Fetching water value...");
    while (fbdo.dataType() != "int") {
      delay(100);
    }
    int waterValue = fbdo.intData();
    Serial.print("Water value from FB: ");
    Serial.println(waterValue);

    // Display water value using LED
    if (waterValue > 0) {
      digitalWrite(ledPin, HIGH); // Turn on LED
    } else {
      digitalWrite(ledPin, LOW); // Turn off LED
    }
  } else {
    Serial.println("Failed to fetch water value");
    Serial.println("Reason: " + String(fbdo.errorReason()));
  }

  delay(5000); // Adjust delay as needed
}

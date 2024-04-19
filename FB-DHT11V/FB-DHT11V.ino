#include <Arduino.h>
#include <DHT.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"  // Include the TokenHelper header file

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

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;

#define DHTPIN 4      // Pin connected to the DHT sensor
#define DHTTYPE DHT11 // Type of DHT sensor (DHT11 or DHT22)

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
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

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Set the user email and password (required) */
  auth.user.email = "leonjosemathew718@gmail.com";
  auth.user.password = "123456@";

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback;

  /* Initialize the library and connect to Firebase */
  Firebase.begin(&config, &auth);

  if (Firebase.ready()) {
    Serial.println("Connected to Firebase");
    signupOK = true;
  } else {
    Serial.println("Failed to connect to Firebase");
    Serial.println("Reason: " + String(fbdo.errorReason()));
  }

  dht.begin();
}

void loop() {
  if (Firebase.ready() && signupOK && millis() - sendDataPrevMillis > 10000) {
    sendDataPrevMillis = millis();

    float temperature = dht.readTemperature();  // Read temperature in Celsius
    float humidity = dht.readHumidity();        // Read humidity

    // Check if any reads failed and exit early (to try again).
    if (isnan(temperature) || isnan(humidity)) {
      Serial.println("Failed to read from DHT sensor!");
      return;
    }

    // Write the temperature and humidity to the database
    if (Firebase.RTDB.setFloat(&fbdo, "Temperature/Celsius", temperature) &&
        Firebase.RTDB.setFloat(&fbdo, "Humidity/Percentage", humidity)) {
      Serial.println("Data sent to Firebase");
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C\t");
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.println("%");
    } else {
      Serial.println("Failed to send data to Firebase");
      Serial.println("Reason: " + String(fbdo.errorReason()));
    }
  }
  delay(1000); // Wait for 1 second before checking again
}

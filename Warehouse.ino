#include <WiFi.h>
#include <FirebaseESP32.h>
#include "DHT.h"

// ----- Wi-Fi -----
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASSWORD "YOUR_WIFI_PASSWORD"

// ----- Firebase -----
#define FIREBASE_HOST "YOUR_FIREBASE_PROJECT.firebaseio.com"
#define FIREBASE_AUTH "YOUR_FIREBASE_DATABASE_SECRET"

FirebaseData firebaseData;

// ----- DHT Sensor -----
#define DHTPIN 4          // GPIO pin
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// ----- Gas Sensor -----
#define GAS_PIN 34        // Analog pin

void setup() {
  Serial.begin(115200);
  
  // Initialize DHT
  dht.begin();
  
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
  
  // Connect to Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Read DHT11
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Read Gas Sensor
  int gasValue = analogRead(GAS_PIN);

  // Print to Serial
  Serial.print("Temp: "); Serial.print(temperature);
  Serial.print(" °C, Humidity: "); Serial.print(humidity);
  Serial.print(" %, Gas: "); Serial.println(gasValue);

  // Send data to Firebase
  if (Firebase.ready()) {
    Firebase.setFloat(firebaseData, "/Warehouse/Temperature", temperature);
    Firebase.setFloat(firebaseData, "/Warehouse/Humidity", humidity);
    Firebase.setInt(firebaseData, "/Warehouse/Gas", gasValue);
  }

  delay(5000); // Update every 5 seconds
}

#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "arduino_secrets.h"
#include <utility/wifi_drv.h>

// Wi-Fi and MQTT details
const char* ssid          = SECRET_SSID;
const char* password      = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;
const char* mqtt_server   = "mqtt.cetools.org";
const int mqtt_port       = 1884;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Change this to your allocated student number
String lightId = "31";

String mqtt_topic =
  "student/CASA0014/luminaire/" + lightId;

String clientId = "";

// LED configuration
const int num_leds = 72;
const int payload_size = num_leds * 3;

byte RGBpayload[payload_size];

// Three photoresistors
const int numberOfSensors = 3;

const int photoresistorPins[numberOfSensors] = {
  A0,
  A1,
  A2
};

// Adjust these values if necessary
const int darkThreshold = 400;
const int lightThreshold = 600;

// Each sensor has its own trigger status
bool readyForCover[numberOfSensors] = {
  true,
  true,
  true
};

int currentColour = 0;

unsigned long lastSensorRead = 0;
const unsigned long sensorInterval = 50;


// --------------------------------------------------
// SETUP
// --------------------------------------------------

void setup() {
  Serial.begin(115200);

  // Analogue input setup
  analogReadResolution(10);

  for (int sensor = 0; sensor < numberOfSensors; sensor++) {
    pinMode(photoresistorPins[sensor], INPUT);
  }

  // Onboard RGB status LED
  WiFiDrv::pinMode(25, OUTPUT);
  WiFiDrv::pinMode(26, OUTPUT);
  WiFiDrv::pinMode(27, OUTPUT);

  LedRed();

  // Create a unique MQTT client ID
  byte mac[6];
  WiFi.macAddress(mac);

  char macStr[18];

  snprintf(
    macStr,
    sizeof(macStr),
    "%02X%02X%02X%02X%02X%02X",
    mac[5],
    mac[4],
    mac[3],
    mac[2],
    mac[1],
    mac[0]
  );

  clientId = "MKR1010_Sender_" + String(macStr);

  Serial.print("Publishing to topic: ");
  Serial.println(mqtt_topic);

  // Connect to Wi-Fi
  startWifi();

  // Connect to MQTT
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setBufferSize(512);
  mqttClient.setKeepAlive(30);

  // Start with red light
  send_colour_to_all(255, 0, 0);

  Serial.println("Setup complete.");
}


// --------------------------------------------------
// MAIN LOOP
// --------------------------------------------------

void loop() {
  // Maintain Wi-Fi connection
  if (WiFi.status() != WL_CONNECTED) {
    startWifi();
  }

  // Maintain MQTT connection
  if (!mqttClient.connected()) {
    reconnectMQTT();
  }

  mqttClient.loop();

  // Read all three photoresistors
  if (millis() - lastSensorRead >= sensorInterval) {
    lastSensorRead = millis();

    for (
      int sensor = 0;
      sensor < numberOfSensors;
      sensor++
    ) {
      int sensorValue =
        analogRead(photoresistorPins[sensor]);

      Serial.print("Photoresistor ");
      Serial.print(sensor + 1);
      Serial.print(" value: ");
      Serial.println(sensorValue);

      // Photoresistor is covered
      if (
        sensorValue < darkThreshold &&
        readyForCover[sensor] == true
      ) {
        readyForCover[sensor] = false;

        currentColour++;

        if (currentColour > 3) {
          currentColour = 0;
        }

        changeLightColour();

        Serial.print("Photoresistor ");
        Serial.print(sensor + 1);
        Serial.println(
          " covered - colour changed."
        );
      }

      // Photoresistor is uncovered again
      if (sensorValue > lightThreshold) {
        readyForCover[sensor] = true;
      }
    }
  }

  delay(50);
}


// --------------------------------------------------
// CHANGE COLOUR
// --------------------------------------------------

void changeLightColour() {
  if (currentColour == 0) {
    Serial.println("Gradient: Red to Purple");

    send_gradient(
      255, 0, 0,
      150, 0, 255
    );
  }

  else if (currentColour == 1) {
    Serial.println("Gradient: Green to Blue");

    send_gradient(
      0, 255, 0,
      0, 0, 255
    );
  }

  else if (currentColour == 2) {
    Serial.println("Gradient: Blue to Pink");

    send_gradient(
      0, 0, 255,
      255, 0, 100
    );
  }

  else if (currentColour == 3) {
    Serial.println("Gradient: Yellow to Red");

    send_gradient(
      255, 255, 0,
      255, 0, 0
    );
  }
}


// --------------------------------------------------
// SEND GRADIENT
// --------------------------------------------------

void send_gradient(
  int startR,
  int startG,
  int startB,
  int endR,
  int endG,
  int endB
) {
  if (mqttClient.connected()) {
    // Display layout: 12 columns x 6 rows
    const int numberOfColumns = 12;

    for (int pixel = 0; pixel < num_leds; pixel++) {
      // Pixels 0-5 = column 1
      // Pixels 6-11 = column 2
      // etc.
      int column = pixel / 6;

      // Position from 0.0 to 1.0
      float position =
        (float)column / (numberOfColumns - 1);

      // Calculate gradient colour
      int r =
        startR + position * (endR - startR);

      int g =
        startG + position * (endG - startG);

      int b =
        startB + position * (endB - startB);

      RGBpayload[pixel * 3 + 0] =
        (byte)constrain(r, 0, 255);

      RGBpayload[pixel * 3 + 1] =
        (byte)constrain(g, 0, 255);

      RGBpayload[pixel * 3 + 2] =
        (byte)constrain(b, 0, 255);
    }

    mqttClient.publish(
      mqtt_topic.c_str(),
      RGBpayload,
      payload_size
    );

    Serial.println("Gradient sent.");
  }

  else {
    Serial.println(
      "MQTT is not connected."
    );
  }
}


// --------------------------------------------------
// SEND ONE COLOUR TO ALL LEDS
// --------------------------------------------------

void send_colour_to_all(
  int r,
  int g,
  int b
) {
  if (mqttClient.connected()) {
    for (int pixel = 0; pixel < num_leds; pixel++) {
      RGBpayload[pixel * 3 + 0] =
        (byte)constrain(r, 0, 255);

      RGBpayload[pixel * 3 + 1] =
        (byte)constrain(g, 0, 255);

      RGBpayload[pixel * 3 + 2] =
        (byte)constrain(b, 0, 255);
    }

    mqttClient.publish(
      mqtt_topic.c_str(),
      RGBpayload,
      payload_size
    );

    Serial.println("MQTT sent.");
  }

  else {
    Serial.println(
      "MQTT is not connected."
    );
  }
}


// --------------------------------------------------
// TURN ALL LEDS OFF
// --------------------------------------------------

void send_all_off() {
  send_colour_to_all(0, 0, 0);
}


#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


const char *ssid = "Home dg";    // Replace with your WiFi SSID
const char *password = "dhruv123"; // Replace with your WiFi password

#define SensorPin A0       // the pH meter Analog output is connected with the Arduino’s Analog
#define MoisturePin D2     // the moisture sensor is connected to digital pin D2

unsigned long int avgValue;  // Store the average value of the sensor feedback
float b;
int buf[10], temp;

ESP8266WebServer server(80);

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Define web server routes
  server.on("/", HTTP_GET, handleRoot);

  // Start server
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Read pH sensor values
  for (int i = 0; i < 10; i++) {
    buf[i] = analogRead(SensorPin);
    delay(10);
  }

  // Sort the analog values
  for (int i = 0; i < 9; i++) {
    for (int j = i + 1; j < 10; j++) {
      if (buf[i] > buf[j]) {
        temp = buf[i];
        buf[i] = buf[j];
        buf[j] = temp;
      }
    }
  }

  avgValue = 0;
  for (int i = 2; i < 8; i++) {
    avgValue += buf[i];
  }

  float phValue = (float)avgValue * 5.0 / 1024 / 6;

  // Read moisture sensor value
  int moistureValue = digitalRead(MoisturePin);

    // Determine soil moisture condition
  String soilCondition;
  String soilCondition_2;
  if (moistureValue == 0) {
    soilCondition = "Moisture Content is good";
  } else {
    soilCondition = "Moisture Content is low";
  }

  if ((5.5 < (phValue)) && ((phValue) < 7)) {
    soilCondition_2 = "Soil is neutral";
  } else if ((phValue) >= 7) {
    soilCondition_2 = "Soil is too basic";
  } else {
    soilCondition_2 = "Soil is too acidic";
  }

  digitalWrite(13, HIGH);
  delay(800);
  digitalWrite(13, LOW);

  String html = "<html><head>";
  html += "<meta http-equiv='refresh' content='5'>";  // Automatic refresh every 5 seconds
  html += "</head><body>";
  html += "<h1>pH Reading:</h1>";
  html += "<p>" + String(phValue, 2) + "</p>";
  html += "<h1>Soil Condition:</h1>";
  html += "<h5>Moisture Reading:</h5>";
  html += "<p>" + soilCondition + "</p>";
  html += "<h5>pH Value:</h5>";
  html += "<p>" + soilCondition_2 + "</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}
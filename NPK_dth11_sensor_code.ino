#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

const char *ssid = "Home dg";
const char *password = "dhruv123";

const int npkSensorPin = A0;           // Analog pin to which the NPK sensor is connected
const int dhtSensorPin = D4;           // Digital pin to which the DHT sensor is connected

DHT dht(dhtSensorPin, DHT11);           // DHT sensor is connected to digital pin D4

ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Initialize DHT sensor
  dht.begin();

  // Define web server routes
  server.on("/", HTTP_GET, handleRoot);

  // Start server
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  // Read NPK sensor analog value
  int npkreading = analogRead(npkSensorPin);

  // Read DHT sensor values
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String soilCondition_1;
  String soilCondition_2;
  String soilCondition_3;
  String soilCondition_4;
  String soilCondition_5;
  
  if ((200 < (npkreading + 200)) && ((npkreading + 200) < 300)) {
    soilCondition_1 = "Nitrogen content is good";
  } else if ((npkreading + 200) >= 300) {
    soilCondition_1 = "Nitrogen content is higher";
  } else {
    soilCondition_1 = "Nitrogen content is lower";
  }

  // Potassium condition
  if ((200 < (npkreading + 268)) && ((npkreading + 268) < 400)) {
    soilCondition_2 = "Potassium content is good";
  } else if ((npkreading + 268) >= 400) {
    soilCondition_2 = "Potassium content is higher";
  } else {
    soilCondition_2 = "Potassium content is lower";
  }

  // Phosphorus reading
  if (30 < (npkreading + 32) && (npkreading + 32) < 60) {
    soilCondition_3 = "Phosphorus content is good";
  } else if ((npkreading + 32) >= 60) {
    soilCondition_3 = "Phosphorus content is higher";
  } else {
    soilCondition_3 = "Phosphorus content is lower";
  }

  // humidity condition
  if (40 < (humidity) && (humidity) < 80) {
    soilCondition_4 = "humidiy is good";
  } else if ((humidity) >= 80) {
    soilCondition_4 = "humidity is too high";
  } else {
    soilCondition_4 = "humidity is too low";
  }

  // humidity condition
  if (15 < (temperature) && (temperature) < 35) {
    soilCondition_5 = "temperature is good";
  } else if ((temperature) >= 35) {
    soilCondition_5 = "temperature is high";
  } else {
    soilCondition_5 = "temperature is low";
  }

  String html = "<html><head>";
  html += "<meta http-equiv='refresh' content='5'>";  // Automatic refresh every 5 seconds
  html += "</head><body>";
  html += "<h1>NPK Readings:</h1>";
  html += "<p>Nitrogen Reading: " + String(npkreading+200) + " mg/Kg</p>";
  html += "<p>Potasium Reading: " + String(npkreading+268) + " mg/Kg</p>";
  html += "<p>Phosphorus Reading: " + String(npkreading+32) + " mg/Kg</p>";
  html += "<h1>DTH Reading:</h1>";
  html += "<p>Temperature: " + String(temperature) + " °C</p>";
  html += "<p>Humidity: " + String(humidity) + "%</p>";
  html += "<h1>NPK Results:</h1>";
  html += "<p>" + soilCondition_1 + "</p>";
  html += "<p>" + soilCondition_2 + "</p>";
  html += "<p>" + soilCondition_3 + "</p>";
  html += "<h1>DTH Results:</h1>";
  html += "<p>" + soilCondition_4 + "</p>";
  html += "<p>" + soilCondition_5 + "</p>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

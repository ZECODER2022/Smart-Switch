#include <WiFiS3.h>
#include <WiFiClientSecure.h>

const char* ssid = "wifiname";
const char* password = "wifipassword";
const char* apiKey = "goveeApiKey";

// Zones

const char* macZone1 = "MacAddress1";
const char* modelZone1 = "HXXXX";

const char* macZone2 = "MacAddress2";
const char* modelZone2 = "HXXXX";

const char* macZone3 = "MacAddress3";
const char* modelZone3 = "HXXXX";

// Buttons

const int btnZone1 = 2;
const int btnZone2 = 3;
const int btnZone3 = 4;
const int btnBrightLow = 5;
const int btnBrightHigh = 6;

const char* activeMac = macZone1;
const char* activeModel = modelZone1;

WiFiClientSecure client;

void setup() {
  Serial.begin(9600);

  pinMode(btnZone1, INPUT_PULLUP);
  pinMode(btnZone2, INPUT_PULLUP);
  pinMode(btnZone3, INPUT_PULLUP);
  pinMode(btnBrightLow, INPUT_PULLUP);
  pinMode(btnBrightHigh, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500; }
}

void loop() {
  if (digitalRead(btnZone1) == LOW) {
    activeMac = macZone1;
    activeModel = modelZone1;
    Serial.println("Zone 1 Selected - Turning On...");
    sendGoveeCommand(activeMac, activeModel, "turn", "on");
    delay(500);
  }

  if (digitalRead(btnZone2) == LOW) {
  activeMac = macZone2;
  activeModel = modelZone2;
  Serial.println("Zone 2 Selected - Turning On...");
  sendGoveeCommand(activeMac, activeModel, "turn", "on");
  delay(500);
  }

  if (digitalRead(btnZone3) == LOW) {
    activeMac = macZone3;
    activeModel = modelZone3;
    Serial.println("Zone 3 Selected - Turning On...");
    sendGoveeCommand(activeMac, activeModel, "turn"m "on");
    delay(500);
  }

  if (digitalRead(btnBrightHigh) == LOW) {
    Serial.println("Settings Brightness to High...");
    sendGoveeCommand(activeMac, activeModel, "brightness", "100");
    delay(500);
  }

  if (digitalRead(btnBrightLow) == LOW) {
    Serial.println("Setting Brightness to Low...");
    sendGoveeCommand(activeMac, activeModel, "brightness", "100");
    delay(500);
  }
}

void sendGoveeCommand(const char* mac, const char* model, String cmdName, String cmdValue) {
  if (client.connect("developer.govee.com", 443)) {
    String valueField = (cmdName == "brightness") ? cmdValue : "\"" + cmdValue + "\"";
    String payload = "{\"device\":\"" + String(mac) + "\",\"model\":\"" + String(model) + "\", \"cmd\":{\"name\":\"" + cmdName + "\",\"value\":" + valueField + "}}";

    client.println("PUT /v1/devices/control HTTP/1.1");
    client.println("Host: developer.govee.com");
    client.print("Govee-API-Key: "); client.println(apiKey);
    client.println("Content-Type: application/json");
    client.println();
    client.println(payload);

    client.stop();
  }
}
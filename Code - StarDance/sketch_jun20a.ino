#include <WiFiS3.h>

const char* ssid = "WIFI_NAME(SSID)";
const char* password = "WIFI_PASSWROD";
const char* apiKey = "GOVEE_API";

// Zones (real devices from your account)

const char* macZone1 = "MAC_ADDRESS";  // Nightstand Lamp
const char* modelZone1 = "HXXXX";

const char* macZone2 = "MAC_ADDRESS";  // Corner Lamp
const char* modelZone2 = "HXXXX";

const char* macZone3 = "MAC_ADDRESS";  // TV Light Bars
const char* modelZone3 = "HXXXX";

// Buttons

const int btnZone1 = 2;
const int btnZone2 = 3;
const int btnZone3 = 4;
const int btnBrightLow = 5;
const int btnBrightHigh = 6;

const char* activeMac = macZone1;
const char* activeModel = modelZone1;

// On/off state for each zone (for toggle behavior)
bool zone1On = false;
bool zone2On = false;
bool zone3On = false;

// Previous button states for edge detection
int lastZone1 = HIGH, lastZone2 = HIGH, lastZone3 = HIGH;
int lastBrightHigh = HIGH, lastBrightLow = HIGH;

WiFiSSLClient client;

void setup() {
  Serial.begin(9600);

  pinMode(btnZone1, INPUT_PULLUP);
  pinMode(btnZone2, INPUT_PULLUP);
  pinMode(btnZone3, INPUT_PULLUP);
  pinMode(btnBrightLow, INPUT_PULLUP);
  pinMode(btnBrightHigh, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.println("WiFi connected");
}

void loop() {
  int z1 = digitalRead(btnZone1);
  int z2 = digitalRead(btnZone2);
  int z3 = digitalRead(btnZone3);
  int bH = digitalRead(btnBrightHigh);
  int bL = digitalRead(btnBrightLow);

  // Zone 1 - toggle on/off
  if (z1 == LOW && lastZone1 == HIGH) {
    activeMac = macZone1; activeModel = modelZone1;
    zone1On = !zone1On;
    if (zone1On) {
      Serial.println("Zone 1 - Turning On...");
      sendGoveeCommand(macZone1, modelZone1, "turn", "on");
    } else {
      Serial.println("Zone 1 - Turning Off...");
      sendGoveeCommand(macZone1, modelZone1, "turn", "off");
    }
  }

  // Zone 2 - toggle on/off
  if (z2 == LOW && lastZone2 == HIGH) {
    activeMac = macZone2; activeModel = modelZone2;
    zone2On = !zone2On;
    if (zone2On) {
      Serial.println("Zone 2 - Turning On...");
      sendGoveeCommand(macZone2, modelZone2, "turn", "on");
    } else {
      Serial.println("Zone 2 - Turning Off...");
      sendGoveeCommand(macZone2, modelZone2, "turn", "off");
    }
  }

  // Zone 3 - toggle on/off
  if (z3 == LOW && lastZone3 == HIGH) {
    activeMac = macZone3; activeModel = modelZone3;
    zone3On = !zone3On;
    if (zone3On) {
      Serial.println("Zone 3 - Turning On...");
      sendGoveeCommand(macZone3, modelZone3, "turn", "on");
    } else {
      Serial.println("Zone 3 - Turning Off...");
      sendGoveeCommand(macZone3, modelZone3, "turn", "off");
    }
  }

  // Brightness High - affects active zone
  if (bH == LOW && lastBrightHigh == HIGH) {
    Serial.println("Setting Brightness to High...");
    sendGoveeCommand(activeMac, activeModel, "brightness", "100");
  }

  // Brightness Low - affects active zone
  if (bL == LOW && lastBrightLow == HIGH) {
    Serial.println("Setting Brightness to Low...");
    sendGoveeCommand(activeMac, activeModel, "brightness", "20");
  }

  lastZone1 = z1; lastZone2 = z2; lastZone3 = z3;
  lastBrightHigh = bH; lastBrightLow = bL;

  delay(50);  // simple debounce
}

void sendGoveeCommand(const char* mac, const char* model, String cmdName, String cmdValue) {
  // Make sure WiFi is still up; reconnect if it dropped
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost - reconnecting...");
    WiFi.begin(ssid, password);
    unsigned long wifiTimeout = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - wifiTimeout < 10000) {
      delay(500);
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("WiFi reconnect failed");
      return;
    }
    Serial.println("WiFi reconnected");
  }

  String valueField = (cmdName == "brightness") ? cmdValue : "\"" + cmdValue + "\"";
  String payload = "{\"device\":\"" + String(mac) + "\",\"model\":\"" + String(model) + "\",\"cmd\":{\"name\":\"" + cmdName + "\",\"value\":" + valueField + "}}";

  // Try to connect up to 3 times (R4's SSL client can fail transiently)
  bool connected = false;
  for (int attempt = 1; attempt <= 3; attempt++) {
    if (client.connect("developer-api.govee.com", 443)) {
      connected = true;
      break;
    }
    Serial.print("Connect attempt ");
    Serial.print(attempt);
    Serial.println(" failed, retrying...");
    delay(500);
  }

  if (!connected) {
    Serial.println("Connection to Govee failed after 3 tries");
    return;
  }

  client.println("PUT /v1/devices/control HTTP/1.1");
  client.println("Host: developer-api.govee.com");
  client.print("Govee-API-Key: "); client.println(apiKey);
  client.println("Content-Type: application/json");
  client.print("Content-Length: "); client.println(payload.length());
  client.println("Connection: close");
  client.println();
  client.print(payload);

  // Read and print the server's response
  unsigned long timeout = millis();
  while (millis() - timeout < 5000) {
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
      timeout = millis();
    }
    if (!client.connected() && !client.available()) break;
  }
  Serial.println();
  client.stop();
}

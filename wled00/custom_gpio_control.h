// File: wled00/custom_gpio_control.h
#ifndef CUSTOM_GPIO_CONTROL_H
#define CUSTOM_GPIO_CONTROL_H

#include "wled.h"

class CustomGPIOControlUsermod : public Usermod {
private:
  // GPIO pins for external devices
  const int COOLERS_PIN = 1;
  const int LAMPS_PIN = 2;
  const int NEON_PIN = 3;

  // States
  bool coolersOn = false;
  bool lampsOn = false;
  bool neonOn = false;

public:
  void setup() {
    pinMode(COOLERS_PIN, OUTPUT);
    pinMode(LAMPS_PIN, OUTPUT);
    pinMode(NEON_PIN, OUTPUT);
    digitalWrite(COOLERS_PIN, LOW);
    digitalWrite(LAMPS_PIN, LOW);
    digitalWrite(NEON_PIN, LOW);
  }

  void loop() {
    // Nothing in main loop, we use WLED UI callbacks
  }

  void addToJsonInfo(JsonObject& root) {
    JsonObject user = root["user"];
    if (user.isNull()) user = root.createNestedObject("user");
    user["coolers"] = coolersOn ? "ON" : "OFF";
    user["lamps"] = lampsOn ? "ON" : "OFF";
    user["neon"] = neonOn ? "ON" : "OFF";
  }

  void addToConfig(JsonObject& root) {
    JsonObject top = root.createNestedObject("CustomGPIOControl");
    top["coolers"] = coolersOn;
    top["lamps"] = lampsOn;
    top["neon"] = neonOn;
  }

  bool readFromConfig(JsonObject& root) {
    JsonObject top = root["CustomGPIOControl"];
    if (top.isNull()) return false;
    coolersOn = top["coolers"] | false;
    lampsOn = top["lamps"] | false;
    neonOn = top["neon"] | false;
    digitalWrite(COOLERS_PIN, coolersOn);
    digitalWrite(LAMPS_PIN, lampsOn);
    digitalWrite(NEON_PIN, neonOn);
    return true;
  }

  void appendConfigData() {
    // Allows toggling from HTTP API or JSON
    server.on("/coolers", HTTP_GET, [this](AsyncWebServerRequest *request){
      coolersOn = !coolersOn;
      digitalWrite(COOLERS_PIN, coolersOn);
      request->send(200, "text/plain", coolersOn ? "ON" : "OFF");
    });
    server.on("/lamps", HTTP_GET, [this](AsyncWebServerRequest *request){
      lampsOn = !lampsOn;
      digitalWrite(LAMPS_PIN, lampsOn);
      request->send(200, "text/plain", lampsOn ? "ON" : "OFF");
    });
    server.on("/neon", HTTP_GET, [this](AsyncWebServerRequest *request){
      neonOn = !neonOn;
      digitalWrite(NEON_PIN, neonOn);
      request->send(200, "text/plain", neonOn ? "ON" : "OFF");
    });
  }
};

#endif

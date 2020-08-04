/*
  OTAUpdate.ino, Example for the AutoConnect library.
  Copyright (c) 2020, Hieromon Ikasamo
  https://github.com/Hieromon/AutoConnect
  This example is an implementation of a lightweight update feature
  that updates the ESP8266's firmware from your web browser.
  You need a compiled sketch binary file to the actual update and can
  retrieve it using Arduino-IDE menu: [Sketck] -> [Export compiled binary].
  Then you will find the .bin file in your sketch folder. Select the.bin
  file on the update UI page to update the firmware.

  Notes:
  If you receive a following error, you need reset the module before sketch running.
  Update error: ERROR[11]: Invalid bootstrapping state, reset ESP8266 before updating.
  Refer to https://hieromon.github.io/AutoConnect/faq.html#hang-up-after-reset for details.

  This software is released under the MIT License.
  https://opensource.org/licenses/MIT
*/

#if defined(ARDUINO_ARCH_ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
typedef ESP8266WebServer WiFiWebServer;
#elif defined(ARDUINO_ARCH_ESP32)
#include <WiFi.h>
#include <WebServer.h>
typedef WebServer WiFiWebServer;
#endif
#include <AutoConnect.h>
#include <config.h>

bool commande_1_pressed = false;

void IRAM_ATTR isr()
{
  commande_1_pressed = !commande_1_pressed;
  if (commande_1_pressed)
  {
    digitalWrite(RELAY_2, HIGH);
  }
  else
  {
    digitalWrite(RELAY_2, LOW);
  }
}

WiFiWebServer server;
AutoConnect portal(server);
AutoConnectConfig config;

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();

  Serial.println("Hello world");

  // Responder of root page and apply page handled directly from WebServer class.

  server.on("/", []() {
    String content = R"(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8" name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
Place the root page with the sketch application.&ensp;
__AC_LINK__
</body>
</html>
    )";
    content.replace("__AC_LINK__", String(AUTOCONNECT_LINK(COG_16)));
    server.send(200, "text/html", content);
  });

  config.ota = AC_OTA_BUILTIN;
  portal.config(config);
  portal.begin();

  // Relays
  Serial.println("config relay_1");
  pinMode(RELAY_1, OUTPUT);
  Serial.println("config relay_2");
  pinMode(RELAY_2, OUTPUT);

  //Telecommande
  pinMode(COMMAND_1, INPUT);
  attachInterrupt(COMMAND_1, isr, FALLING);
}

void loop()
{
  portal.handleClient();

  Serial.println("commande relay_1");
  digitalWrite(RELAY_1, HIGH);
  //digitalWrite(RELAY_2,LOW);
  sys_delay_ms(500);
  Serial.println("commande relay_2");
  digitalWrite(RELAY_1, LOW);
  //digitalWrite(RELAY_2,HIGH);
  sys_delay_ms(500);
  
}
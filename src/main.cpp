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
#include <cmd_relay.hpp>

WiFiWebServer server;
AutoConnect portal(server);
AutoConnectConfig config;

bool whileCP(void) {
  bool  rc;
  // Here, something to process while the captive portal is open.
  // To escape from the captive portal loop, this exit function returns false.
  // rc = true;, or rc = false;
  return rc;
}

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println();

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
  config.autoReconnect = true;
  //config.reconnectInterval = 6;   // Seek interval time is 180[s].
  //config.retainPortal = true;   // Keep the captive portal open.
  //config.beginTimeout = 15000; // Timeout sets to 15[s]
  portal.whileCaptivePortal(whileCP);
  Serial.println("config");
  portal.config(config);
  //portal.begin();
    Serial.println("begin");


  // Establish a connection with an autoReconnect option.
  if (portal.begin()) {
    Serial.println("WiFi connected: " + WiFi.localIP().toString());
  } 
  Serial.println("setup_cmd_shutter");
  setup_cmd_shutter();
}

void loop()
{
  if (WiFi.status() == WL_CONNECTED) {
    // Here to do when WiFi is connected.
    //Serial.println("run_conected");
    run();
  }
  else {
    // Here to do when WiFi is not connected.
    //Serial.println("run_not_conected");
    run();
  }
  
  portal.handleClient();
}

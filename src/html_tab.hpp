#ifndef HTML_TAB_H
#define HTML_TAB_H

#include <WiFi.h>
#include <WebServer.h>
#include <AutoConnect.h>

void html_tab_init(WebServer& server, AutoConnect& portal);

#endif
#include <html_tab.hpp>
#include <cmd_relay.hpp>

#if defined(ARDUINO_ARCH_ESP32)
#include <FS.h>
#include <SPIFFS.h>
#include <WebServer.h>
using WebServerClass = WebServer;
fs::SPIFFSFS& FlashFS = SPIFFS;
#define FORMAT_ON_FAIL  true
#endif

#define COMMAND_URL      "/command"
#define VOLET_DOWN     "/_volet_down"
#define VOLET_UP       "/_volet_up"
#define VOLET_STOP       "/_volet_stop"
#define VERSION "1.2 debug"

static const char PAGE_COMMAND[] PROGMEM = R"*(
{
  "uri": "/command",
  "title": "Configuration",
  "menu": true,
  "element": [
    {
      "name": "tablecss",
      "type": "ACStyle",
      "value": "table{font-family:arial,sans-serif;border-collapse:collapse;width:100%;color:black;}td,th{border:1px solid #dddddd;text-align:center;padding:8px;}tr:nth-child(even){background-color:#dddddd;}"
    },
    {
      "name": "version",
      "type": "ACText",
      "format": "Version : %s<br />"
    },
    {
      "name": "up",
      "type": "ACButton",
      "value": "  UP  ",
      "action": "_voletup(this)"
    },
    {
      "name": "_voletup",
      "type": "ACElement",
      "value": "<script>function _voletup(t){const el=new FormData();el.append(t.name,t.value);fetch(')*" VOLET_UP
     R"*(',{mode:'no-cors',method:'post',body:el}).then(res=>{if(res.ok){return res.text();}}).then(text=>{console.log(text);const anc=document.getElementById(t.id);anc.value=text;anc.innerHTML=text;}).catch(err=>console.log(err));}</script>"
    },
    {
      "name": "stop",
      "type": "ACButton",
      "value": "  STOP  ",
      "action": "_voletstop(this)"
    },
    {
      "name": "_voletstop",
      "type": "ACElement",
      "value": "<script>function _voletstop(t){const el=new FormData();el.append(t.name,t.value);fetch(')*" VOLET_STOP
     R"*(',{mode:'no-cors',method:'post',body:el}).then(res=>{if(res.ok){return res.text();}}).then(text=>{console.log(text);const anc=document.getElementById(t.id);anc.value=text;anc.innerHTML=text;}).catch(err=>console.log(err));}</script>"
    },
    {
      "name": "down",
      "type": "ACButton",
      "value": " DOWN ",
      "action": "_voletdown(this)"
    },
    {
      "name": "_voletdown",
      "type": "ACElement",
      "value": "<script>function _voletdown(t){const el=new FormData();el.append(t.name,t.value);fetch(')*" VOLET_DOWN R"*(',{mode:'no-cors',method:'post',body:el}).then(res=>{if(res.ok){return res.text();}}).then(text=>{console.log(text);const anc=document.getElementById(t.id);anc.value=text;anc.innerHTML=text;}).catch(err=>console.log(err));}</script>"
    },
    {
      "name": "adjust_width",
      "type": "ACElement",
      "value": "<script type=\"text/javascript\">window.onload=function(){var t=document.querySelectorAll(\"input[type='text']\");for(i=0;i<t.length;i++){var e=t[i].getAttribute(\"placeholder\");e&&t[i].setAttribute(\"size\",e.length*.8)}};</script>"
    }
  ]
}
)*";


void VoletUp() 
{
  set_up();
}

void  VoletDown() 
{
  set_down();
}
void  VoletStop() 
{
  set_stop();
}

void html_tab_init(WebServer& server, AutoConnect& portal)
{
  portal.load(PAGE_COMMAND);

  AutoConnectAux* debug_page = portal.aux(COMMAND_URL);

  AutoConnectText&  version = debug_page->getElement<AutoConnectText>("version");
  version.value = VERSION;

  AutoConnectButton&  button_up = debug_page->getElement<AutoConnectButton>("up");
  AutoConnectButton&  button_down = debug_page->getElement<AutoConnectButton>("down");
  AutoConnectButton&  button_stop = debug_page->getElement<AutoConnectButton>("stop");

  server.on (VOLET_UP, VoletUp);
  server.on(VOLET_DOWN, VoletDown);
  server.on(VOLET_STOP, VoletStop);
}
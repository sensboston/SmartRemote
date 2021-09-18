#define USE_OTA
#define USE_WEB_SERVER
//#define USE_DISPLAY
//#define USE_MQTT

#ifndef USE_MQTT
#define USE_SINRIC_PRO
#endif

#include <WiFi.h>
#include <IRremote.h>

#ifdef USE_OTA
#include <ArduinoOTA.h>
#endif

#ifdef USE_WEB_SERVER
#include <WebServer.h>
#endif

#ifdef USE_DISPLAY
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#endif

#ifdef USE_MQTT
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#else
#ifdef USE_SINRIC_PRO
#include "SinricPro.h"
#include "SinricProWindowAC.h"
#endif
#endif

#define WIFI_HOST       "SmartRemote"
#define WIFI_SSID       "<YOUR_WIFI_SSID>"
#define WIFI_PASS       "<YOUR_WIFI_PASSWORD>"

#ifdef USE_MQTT
#define IO_SERVER       "io.adafruit.com"
#define IO_SERVERPORT   1883                  
#define IO_USERNAME     "<YOUR_IO_USERNAME>"
#define IO_KEY          "<YOUR_IO_KEY>"
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);       
Adafruit_MQTT_Subscribe AC_Control = Adafruit_MQTT_Subscribe(&mqtt, IO_USERNAME"/feeds/bedroom-ac");
Adafruit_MQTT_Subscribe *subscription;
#else
#ifdef USE_SINRIC_PRO
#define APP_KEY         "<YOUR_SINRIC_KEY>"                                       
#define APP_SECRET      "<YOUR_SINRIC_SECRET>"
#define ACUNIT_ID       "<YOUR_SINRIC_DEVICE_ID>"
#endif
#endif

#define IR_SEND_PIN 4

#ifdef USE_DISPLAY
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#endif

const uint8_t power_cmd[] PROGMEM = {181,89, 12,11, 12,10, 12,10, 12,33, 12,11, 12,10, 12,11, 11,11, 12,33, 12,10, 12,33, 12,10, 12,33, 12,33, 12,33, 12,33, 12,33, 12,10, 12,10, 12,11, 12,33, 12,10, 12,11, 11,11, 12,10, 12,33, 12,33, 12,33, 12,10, 12,33, 12,33, 12,33, 12};
const uint8_t temp_up_cmd[] PROGMEM = {181,89, 12,11, 12,10, 12,11, 12,32, 12,11, 12,10, 12,11, 12,10, 12,33, 12,10, 12,33, 12,11, 12,32, 12,34, 11,33, 12,33, 12,10, 12,33, 12,33, 12,33, 12,10, 12,11, 12,10, 12,10, 12,33, 12,11, 12,10, 12,11, 11,33, 12,33, 12,33, 12,33, 12};
const uint8_t temp_down_cmd[] PROGMEM = {180,90, 12,10, 12,10, 12,11, 12,33, 12,10, 12,11, 11,11, 12,10, 12,33, 12,11, 12,32, 12,11, 12,33, 12,32, 12,33, 12,33, 12,33, 12,10, 12,33, 12,33, 12,10, 12,11, 12,10, 12,11, 12,10, 12,33, 12,10, 12,11, 12,33, 11,34, 11,33, 12,33, 12};
const uint8_t fan_slower_cmd[] PROGMEM = {181,89, 12,11, 12,10, 12,11, 11,34, 11,11, 12,10, 12,11, 11,11, 12,33, 12,10, 12,33, 12,11, 11,34, 11,34, 11,33, 12,33, 12,10, 12,11, 12,32, 12,11, 12,10, 12,11, 12,10, 12,11, 11,33, 12,33, 12,11, 12,32, 12,33, 12,33, 12,33, 12,33, 12};
const uint8_t fan_faster_cmd[] PROGMEM = {180,89, 12,11, 12,10, 12,11, 12,33, 11,11, 12,10, 12,11, 12,10, 12,33, 12,10, 12,33, 12,11, 12,33, 11,33, 12,33, 12,33, 12,33, 12,10, 12,11, 12,10, 12,10, 12,11, 12,10, 12,11, 12,10, 12,33, 12,33, 12,33, 11,33, 12,33, 12,33, 12,33, 12};
const uint8_t cool_cmd[67] PROGMEM = {180,90, 11,11, 12,10, 12,11, 12,33, 11,11, 12,10, 12,11, 12,10, 12,34, 11,11, 11,34, 11,11, 12,33, 11,34, 11,34, 11,33, 12,33, 12,10, 12,11, 12,33, 11,11, 12,10, 12,11, 12,10, 12,11, 11,34, 11,34, 11,11, 12,33, 11,34, 11,34, 11,33, 12};
const uint8_t energy_cmd[] PROGMEM = {181,89, 11,12, 11,11, 12,10, 12,34, 11,11, 11,11, 12,10, 12,11, 12,33, 12,10, 12,33, 12,10, 12,34, 11,33, 12,33, 12,33, 12,10, 12,33, 12,10, 12,11, 12,10, 12,11, 11,11, 12,10, 12,33, 12,11, 11,34, 11,34, 11,34, 11,33, 12,33, 12,33, 11};
const uint8_t fan_only_cmd[] PROGMEM = {181,89, 12,11, 12,10, 12,11, 12,33, 11,11, 12,10, 12,11, 12,10, 12,33, 12,10, 12,34, 11,11, 12,33, 11,33, 12,34, 11,33, 12,33, 12,33, 11,34, 11,11, 12,10, 12,11, 12,10, 12,11, 11,11, 12,10, 12,11, 12,33, 11,34, 11,33, 12,34, 11,33, 12};
const uint8_t sleep_cmd[] PROGMEM = {180,90, 11,11, 12,10, 12,11, 12,33, 11,11, 12,10, 12,11, 12,10, 12,34, 11,11, 11,33, 12,11, 12,33, 11,34, 11,34, 11,34, 11,10, 12,11, 12,10, 12,11, 11,11, 12,11, 11,11, 12,10, 12,34, 11,33, 12,33, 12,33, 12,33, 11,34, 11,34, 11,33, 12};
const uint8_t auto_cmd[] PROGMEM = {180,90, 11,11, 12,10, 12,11, 12,33, 12,10, 12,10, 12,11, 12,10, 12,33, 12,10, 12,33, 12,11, 12,33, 11,33, 12,33, 12,33, 12,33, 12,33, 12,33, 11,33, 12,11, 12,10, 12,11, 12,10, 12,10, 12,11, 12,10, 12,11, 12,32, 12,33, 12,33, 12,33, 12};
const uint8_t timer_cmd[] PROGMEM = {181,89, 12,11, 12,10, 12,10, 12,33, 12,11, 12,10, 12,10, 12,11, 12,33, 12,10, 12,33, 12,10, 12,34, 11,33, 12,33, 12,33, 12,10, 12,33, 12,33, 12,10, 12,11, 11,11, 12,10, 12,11, 12,33, 12,10, 12,10, 12,34, 11,33, 12,33, 12,33, 12,33, 12};
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))
#define rawSize ARRAY_SIZE(power_cmd)

#ifdef USE_WEB_SERVER
WebServer server(80);
#define GO_BACK server.sendHeader("Location", "/",true); server.send(302, "text/plane","");

// This is HTML page for settings
const char index_html[] PROGMEM = R"rawliteral(<!DOCTYPE html>
<head>
  <title>Smart AC remote control</title>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0" />
  <meta name="mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-capable" content="yes">
  <link rel="icon" href="http://senssoft.com/ac.ico" type="image/x-icon"/>
  <link rel="shortcut icon" href="http://senssoft.com/ac.ico" type="image/x-icon"/>  
  <link rel="apple-touch-icon" href="http://senssoft.com/img/ac.png"/>
  <script type="text/javascript" src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
  <script type="text/javascript" src="http://senssoft.com/scripts/maphilight.min.js"></script>
  <script type="text/javascript">
    $.fn.maphilight.defaults = {fill:true,fillColor:'00FF00',fillOpacity:0.1,stroke:false,strokeColor:'ff0000',
    strokeOpacity:0,strokeWidth:0,fade:true,alwaysOn:false,neverOn:false,groupBy:false,wrapClass:true,shadow:false,
    shadowX:0,shadowY:0,shadowRadius:6,shadowColor:'000000',shadowOpacity:0,shadowPosition:'outside',shadowFrom: false}
    $(function() { $(".map").maphilight();});
  </script>
  <style>
    div { 
        display:grid; 
        place-items:center; 
        align-content:center;
    }
    button {
        display:grid; 
        place-items:center; 
        align-content:center;
        color: #FFFFFF;
        background-color: #3333FF;
        font-size: 18px;
        cursor: pointer;
        text-align: center;
        margin-top: 16px;
        height: 40px;
        width: 420px;
    }    
  </style>
</head>
<body style="touch-action: pan-x pan-y;">
    <div style="zoom:.6;-o-transform: scale(.6);-moz-transform: scale(.6)">
        <img class="map" src="http://senssoft.com/img/ac_remote.png" usemap="#image-map">
    </div>
    <map name="image-map">
        <area onclick="window.location.replace('/command?cmd=power');" coords="108,38,198,128" shape="rect">
        <area onclick="window.location.replace('/command?cmd=temp_up');" coords="183,263,131,213,134,202,148,196,426,202,442,208,442,218,395,267" shape="poly">
        <area onclick="window.location.replace('/command?cmd=fan_faster');" coords="410,284,459,234,471,234,477,248,478,518,470,536,462,542,408,491" shape="poly">
        <area onclick="window.location.replace('/command?cmd=temp_down');" coords="187,506,391,507,443,559,439,565,426,570,151,570,137,563,136,552" shape="poly">
        <area onclick="window.location.replace('/command?cmd=fan_slower');" coords="166,279,167,486,115,538,106,534,101,520,101,238,108,228,117,227" shape="poly">
        <area onclick="window.location.replace('/command?cmd=cool');" coords="112,680,182,752" shape="rect">
        <area onclick="window.location.replace('/command?cmd=energy');" coords="250,680,322,752" shape="rect">
        <area onclick="window.location.replace('/command?cmd=fan_only');" coords="392,680,462,752" shape="rect">
        <area onclick="window.location.replace('/command?cmd=sleep');" coords="112,810,182,878" shape="rect">
        <area onclick="window.location.replace('/command?cmd=auto');" coords="250,810,322,878" shape="rect">
        <area onclick="window.location.replace('/command?cmd=timer');" coords="392,810,462,878" shape="rect">
        <area coords="0,0,568,1216" shape="rect" data-maphilight='{"fillOpacity":0.0}'> 
    </map>
</body>
</html>)rawliteral";
#endif

int wifiStatus = WL_IDLE_STATUS;
unsigned long prevMillis = 0;

#ifdef USE_DISPLAY
void clearDisplay()
{
    display.clearDisplay();
    display.display();
    display.setCursor(0,0);
    display.display();
    delay(250);
}
#else
void clearDisplay(){}
#endif

void executeCommand(String cmd)
{
    if (cmd=="power") IrSender.sendRaw_P(power_cmd, rawSize, 38);
    else if (cmd=="temp_up") IrSender.sendRaw_P(temp_up_cmd, rawSize, 38);
    else if (cmd=="fan_faster") IrSender.sendRaw_P(fan_faster_cmd, rawSize, 38);
    else if (cmd=="temp_down") IrSender.sendRaw_P(temp_down_cmd, rawSize, 38);
    else if (cmd=="fan_slower") IrSender.sendRaw_P(fan_slower_cmd, rawSize, 38);
    else if (cmd=="cool") IrSender.sendRaw_P(cool_cmd, rawSize, 38);
    else if (cmd=="energy") IrSender.sendRaw_P(energy_cmd, rawSize, 38);
    else if (cmd=="fan_only") IrSender.sendRaw_P(fan_only_cmd, rawSize, 38);
    else if (cmd=="sleep") IrSender.sendRaw_P(sleep_cmd, rawSize, 38);
    else if (cmd=="auto") IrSender.sendRaw_P(auto_cmd, rawSize, 38);
    else if (cmd=="timer") IrSender.sendRaw_P(timer_cmd, rawSize, 38);
}

#ifdef USE_WEB_SERVER
void handleHTTPCommand()
{
    if (server.args() == 1)
    {
        String cmd = server.arg(0);
#ifdef USE_DISPLAY        
        prevMillis = millis();
        wifiStatus = WL_IDLE_STATUS;
        clearDisplay();
        display.setCursor(0,8);
        display.println("Got HTTP command:");
        display.setCursor(0,20);
        display.println(cmd);
        display.display();
#else        
        Serial.print("Got HTTP command: ");
        Serial.println(cmd);
#endif  
        executeCommand(cmd);      
    }
    GO_BACK
}
#endif

#ifdef USE_MQTT
void handleGoogleCommand(String cmd)
{
    cmd.toLowerCase();
    if (cmd.startsWith("on") || cmd.startsWith("off")) cmd = "power";
    else if (cmd.startsWith("fast")) cmd = "fan_faster";
    else if (cmd.startsWith("slow")) cmd = "fan_slower";
    else if (cmd.startsWith("cool") || cmd.indexOf("cool")>=0) cmd = "cool";
    else if (cmd.indexOf("energy")>=0 || cmd.indexOf("saver")>=0) cmd = ("energy");
    else if (cmd.startsWith("auto")) cmd = "auto";
    else if (cmd.startsWith("fan")) cmd = "fan_only";
    else if (cmd.startsWith("sleep")) cmd = "sleep";

#ifdef USE_DISPLAY        
    prevMillis = millis();
    wifiStatus = WL_IDLE_STATUS;
    clearDisplay();
    display.setCursor(0,8);
    display.println("Got from Google:");
    display.setCursor(0,20);
    display.println(cmd);
    display.display();
#else
    Serial.print("Got from Google: ");
    Serial.println(cmd);
#endif        
    executeCommand(cmd);
}
#endif

void waitForWiFiConnectOrReboot()
{
  uint32_t notConnectedCounter = 0;
  while (WiFi.status() != WL_CONNECTED) 
  {
      delay(200);
      notConnectedCounter++;
      if (notConnectedCounter > 30) ESP.restart();
  }
  // Print wifi IP addess
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

#ifdef USE_SINRIC_PRO
float globalTemperature = 18;

bool onPowerState(const String &deviceId, bool &state) 
{
    executeCommand("power");
    return true;
}

bool onTargetTemperature(const String &deviceId, float &temperature) 
{
    executeCommand(temperature < globalTemperature ? "temp_down" : "temp_up");
    globalTemperature = temperature;
    return true;
}

bool onAdjustTargetTemperature(const String & deviceId, float &temperatureDelta) 
{
    return true;
}

bool onThermostatMode(const String &deviceId, String &mode) 
{
    if (mode.indexOf("cool") >=0) executeCommand("cool");
    else if (mode.indexOf("auto") >=0) executeCommand("auto");
    else if (mode.indexOf("saver") >=0) executeCommand("energy");
    return true;
}
#endif

void setup() 
{
    Serial.begin(115200);
        
    WiFi.mode(WIFI_STA);

#ifdef USE_DISPLAY
    display.begin(SSD1306_SWITCHCAPVCC);
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    clearDisplay();
    display.println("- connect to WiFi");
    display.println("hotspot 'SmartRemote'");
    display.println("- open in browser");
    display.println("http://192.168.4.1");
    display.display();
    delay(250);
#endif    

    IrSender.begin(IR_SEND_PIN, true); 

    // Connect to WiFi
    WiFi.setHostname(WIFI_HOST);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    waitForWiFiConnectOrReboot();

    if (WiFi.status() == WL_CONNECTED) 
    {
        Serial.print("IP address: "); Serial.println(WiFi.localIP());
#ifdef USE_OTA
        ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH) type = "sketch"; else type = "filesystem";
                Serial.println("Start updating " + type);
            })
            .onEnd([]() { Serial.println("\nEnd"); })
            .onProgress([](unsigned int progress, unsigned int total) { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
            .onError([](ota_error_t error) { Serial.printf("Error[%u]: ", error); });
    
        ArduinoOTA.setHostname(WIFI_HOST);
        ArduinoOTA.begin();
    }
#endif    

#ifdef USE_WEB_SERVER
    // Setup web server
    server.on("/", []() { server.send(200, "text/html", index_html); } );
    server.on("/command", handleHTTPCommand);
    server.on("/favicon.ico", []() { server.sendHeader("Location", "http://senssoft.com/ac.ico",true); server.send(302, "text/plane",""); });
    server.onNotFound( [](){ GO_BACK });
    server.begin();
#endif    

#ifdef USE_MQTT
    // Subscribe for Adafruit IO feed
    mqtt.subscribe(&AC_Control);
#ifndef USE_DISPLAY
    Serial.println("Connecting to MQTT server...");
#endif
    mqtt.connect();
#else
#ifdef USE_SINRIC_PRO
    // get a new AC unit device from SinricPro
    SinricProWindowAC &myAcUnit = SinricPro[ACUNIT_ID];
    myAcUnit.onPowerState(onPowerState);
    myAcUnit.onTargetTemperature(onTargetTemperature);
    myAcUnit.onAdjustTargetTemperature(onAdjustTargetTemperature);
    myAcUnit.onThermostatMode(onThermostatMode); 

    // setup SinricPro
    SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
    SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
    SinricPro.begin(APP_KEY, APP_SECRET);
#endif
#endif
}

void loop() 
{
    // Check WiFi status every minute
    if(millis()-prevMillis > 5*60*1000)
    {
        if (WiFi.status() != WL_CONNECTED) ESP.restart();
        else prevMillis = millis();
    }

#ifdef USE_OTA    
    ArduinoOTA.handle();
#endif    

    if (WiFi.status() == WL_CONNECTED)
    {
#ifdef USE_WEB_SERVER        
        server.handleClient();
#endif        
    
#ifdef USE_MQTT        
        if (mqtt.connected())
        {
            while ((subscription = mqtt.readSubscription(500))) 
            {
                if (subscription == &AC_Control) 
                    handleGoogleCommand(String ((char*)AC_Control.lastread));
            }
        }
        else
        {
#ifndef USE_DISPLAY
            Serial.println("Connecting to MQTT server...");
#endif
            mqtt.connect();
        }
#else
#ifdef USE_SINRIC_PRO
        SinricPro.handle();
#endif
#endif
    }
}

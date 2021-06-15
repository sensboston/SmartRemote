# SmartRemote
ESP32-based IR remote with Google Home integration

There are lot of similar solutions for Arduino or ESP32 based SoCs; most of 'em aren't useful as a retail universal remotes (sunch as **Broadlink RM3/RM4 mini** you can buy on eBay for $15 or cheaper) because of IR transmitter range and angle: cheap IR transmitter has very limited range (about 2-3 feet) and angle (30 degrees). I.e. from Broadlink IR3 mini you can control all devices in the room; with the custom "arduino" solution you simple can't. 

However if you don't trust proprietary code and foreign servers, or have a troubles to adding  your IR-controlled device to Broadlink RM3 mini (or any other retail unversal remote), this solution might be helpful for you.
I believe, my solution has a few advantages:
- you can easily create a web-based remote control for **literally** any IR-controlled device
- you don't need to be a web designer to create non-ugly web app pages: just follow this simple guide
- it's still pretty budget and lightwight; code is clear and straightforward 

I'll not provide schematics 'cause it's extrimily simple; btw, if you're reading this, you should understand how electronic parts works 😜

You'll need ESP32 dev board (I've used ESP-WROOM-32, you may choose any compatibe or ESP8266-based, with slight changes in the code), [IR receiver & transmitter boards](https://www.ebay.com/itm/353365167100?hash=item5246348bfc:g:IOwAAOSwRnZgDc~M) (as I recommend; btw, you can make IR transmitter and receiver by yourself, google for this). Optionally, you can use I2C OLED display; I've used it 'cause I've purchased a lot of 'em for cheap on AliExpress.

By [IRremote library](https://github.com/z3t0/Arduino-IRremote.git) defaults, pins assigned to 
```
#define IR_RECEIVE_PIN  15  // D15
#define IR_SEND_PIN      4  // D4
```
For I2C SSD1306 display you should install library Adafruit_SSD1306 and connect SCL and SDA pins to ESP32 pins D22 and D21

<img width="400px" src="https://iotdesignpro.com/sites/default/files/inline-images/Interfacing-OLED-Display-with-ESP32-using-Arduino-IDE.png">

So, please follow all steps below to create your own, nice and photorealistic, "smart" remote control.

1) Clone this repo code
2) Connect IR receiver board to your ESP32
3) Copy & paste & run this simple sketch in Arduino IDE:
```
#include <IRremote.h>
#define IR_RECEIVE_PIN 15
void setup() 
{
    Serial.begin(115200);
    IrReceiver.begin(IR_RECEIVE_PIN);
}
void loop() 
{
    if (IrReceiver.decode())
    {
        if (IrReceiver.decodedIRData.numberOfBits > 1) IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, false);
        IrReceiver.resume();
    }
}
```
4) Open "Serial monitor" window in Arduino IDE, point your physical remote you wanna to clone to IR receiver, and press (just once) buttons on the remote, to copy raw IR codes data.
5) Replace ```const uint8_t PROGMEM``` arrays in the "SmartRemote.ino" sketch to your own codes.
6) Make a good picture of your remote; use a Photoshop's or other graphics editors "Lasso" tool to make image clear of background (see my image below):

<img width="200px" src="http://senssoft.com/img/ac_remote.png">

5) Open [that site](https://www.image-map.net/), upload your image and create image map for all remote buttons.
6) Copy HTML source code of generated image map, and adjuust Arduino sketch HTML  and Javascrip code
7) To integrate with Google Home (or Alexa, or Siri), follow [this guide](https://iotdesignpro.com/projects/google-assistant-controlled-led-using-ESP32-and-adafruit-io) (all information is up to date, at least on June, 15, 2021)
8) Assemble your smart remote, flash sketch, attach to AC/TV/gadget by mounting scotch tape and you should be allset. By the way, don't forget to configure WiFi connection finally. You should get something like this (it's my smart remote attached to my old Frigidaire air conditioner

<img width="300px" src="http://senssoft.com/img/remote_pic.jpg">

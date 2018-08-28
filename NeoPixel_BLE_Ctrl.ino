/* 
 *  Neopixel Bluetooth Control with Blynk App
 *  https://www.blynk.cc/
 *  IOS: https://itunes.apple.com/us/app/blynk-control-arduino-raspberry/id808760481?ls=1&mt=8
 *  Andriod: https://play.google.com/store/apps/details?id=cc.blynk
 *  
 *  
 *  ************          Material:          ************
 *  Neopixel Strip (replace 24 with the number of NeoPixel LEDs you have)
 *  Arduino Nano V3 
 *  HM10 Bluetooth module. 
 *  300 to 500 Ohm Resistor 
 *  1K Ohm Resistor 
 *  2K Ohm Resistor 
 *  
 *  
 *  ************      APP Configuration:      ************
 *  Register an account with Blynk 
 *  Create new project 
 *  Pick device as Arduino Nano
 *  Pick connection method as Bluetooth 
 *  Send yourself the auth token
 *  Copy and paste the Auth token below
 *  
 *  Use "Styled Button":  Output as Vitual Pin V0 
 *                        Mode as Push 
 *  
 *  Use "BLE":            Connect to the HM10 module 
 *  
 *  Use "zeRGBa":         Output at MERGE 
 *                        Output as Virtual Pin V1
 *  
 *  
 *  ************          Connection:        ************
 *  HM10 VCC pin ---> Arduino 3.3V
 *  HM10 GND pin ---> Arduino GND
 *  HM10 TX pin ---> Arduino D8 
 *  HM10 RX pin ---> 1K Ohm ---> Arduino D9
 *  HM10 RX pin ---> 2K Ohm ---> GND
 *  Neopixel Data pin ---> 300 to 500 Ohm ---> Arduino D6
 *  Neopixel V+ pin ---> Arduino 5V 
 *     ****(if using more that 30 Neopixel LEDs, 
 *     power the strip with suitable external 5V power supply)****
 *  Neopixel GND pin ---> Arduino GND
 */

// You should get Auth Token in the Blynk App.
char auth[] = "761dcdbfb43a46ec8c1a7392444dda05";
// Change 24 with the number of Neopixel LEDs on your strip
#define NEOPIXEL 24 



#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
SoftwareSerial SwSerial(8, 9); // RX, TX
#include <BlynkSimpleSerialBLE.h>
#include <SoftwareSerial.h>
#include <Adafruit_NeoPixel.h>
SoftwareSerial SerialBLE(8, 9); // RX, TX

#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN);

int state = 0;

BLYNK_WRITE(V0) {
  // On or Off
  state = param.asInt();
  if (state == 1) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
  }
}

BLYNK_WRITE(V1) {
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

void setup() {
  // Debug console
  Serial.begin(9600);

  SerialBLE.begin(9600);
  Blynk.begin(SerialBLE, auth);

  Serial.println("Waiting for connections...");

  strip.begin();
  strip.show();
}

void loop()
{
  Blynk.run();
}


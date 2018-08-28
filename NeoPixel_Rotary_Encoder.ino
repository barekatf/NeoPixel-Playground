#include <Adafruit_NeoPixel.h>

# define PIN 6 
# define button 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, PIN);

int state = 0; 

int buttonState = 0; 

// Encoder functions and variables 
void doEncoderA();
void doEncoderB();
int encoderA = 2;
int encoderB = 3;
volatile bool A_set = false;
volatile bool B_set = false;
volatile int encoderPos = 0;
int encoderPosition = 0; 

int intensity = 0; 

int red = 0; 
int green = 0; 
int blue = 0; 

// color show parameters
int dir = 1; 
int stepTime = 50; 
int showLED = 0; 
int start = 0; 
int finish = 0; 

void setup () { 
    Serial.begin(9600); 
    pinMode(PIN, OUTPUT); // pin for the neopixel 
    pinMode(button, INPUT_PULLUP); 
    // setup the neopixel 
    strip.begin(); 
    strip.show(); 
    // Setup encoder 
    pinMode(encoderA, INPUT_PULLUP);
    pinMode(encoderB, INPUT_PULLUP);
    attachInterrupt(0, doEncoderA, CHANGE);
    attachInterrupt(1, doEncoderB, CHANGE);
}

void loop () {
    switch (state) {
        case 0: 
          // Get intensity for Red 
          buttonState = digitalRead(button); 
          encoderPosition = encoderPos; 
          intensity = map(encoderPosition, 0, 24, 0, 255); 
          for (int i = 0; i < 8; i++) { 
            strip.setPixelColor(i, intensity, 0, 0); 
          }
          for (int i = 8; i < 24; i++) { 
            strip.setPixelColor(i, 0, 0, 0); 
          }
          strip.show(); 

          if (buttonState == LOW) { 
            red = intensity; 
            state = 1; 
          }
          break; 
        case 1: 
          // button debounce state 
          buttonState = digitalRead(button); 
          if (buttonState == HIGH) { 
            encoderPosition = 0; 
            encoderPos = 0; 
            intensity = 0; 
            state = 2; 
          }
          break; 
        case 2: 
          // Get intensity for green 
          buttonState = digitalRead(button); 
          encoderPosition = encoderPos; 
          intensity = map(encoderPosition, 0, 24, 0, 255); 
          for (int i = 0; i < 8; i++) { 
            strip.setPixelColor(i, red, 0, 0); 
          }
          for (int i = 8; i < 16; i++) { 
            strip.setPixelColor(i, 0, intensity, 0); 
          }
          for (int i = 16; i < 24; i++) { 
            strip.setPixelColor(i, 0, 0, 0); 
          }
          strip.show(); 

          if (buttonState == LOW) { 
            green = intensity; 
            state = 3; 
          }
          break; 
        case 3: 
          // button debounce state 
          buttonState = digitalRead(button); 
          if (buttonState == HIGH) { 
            encoderPosition = 0; 
            encoderPos = 0; 
            intensity = 0; 
            state = 4; 
          }
          break; 
        case 4: 
          // Get intensity for blue 
          buttonState = digitalRead(button); 
          encoderPosition = encoderPos; 
          intensity = map(encoderPosition, 0, 24, 0, 255); 
          for (int i = 0; i < 8; i++) { 
            strip.setPixelColor(i, red, 0, 0); 
          }
          for (int i = 8; i < 16; i++) { 
            strip.setPixelColor(i, 0, green, 0); 
          }
          for (int i = 16; i < 24; i++) { 
            strip.setPixelColor(i, 0, 0, intensity); 
          }
          strip.show(); 

          if (buttonState == LOW) { 
            blue = intensity; 
            state = 5; 
          }
          break; 
        case 5: 
          // button debounce state 
          buttonState = digitalRead(button); 
          if (buttonState == HIGH) { 
            encoderPosition = 0; 
            encoderPos = 1; 
            intensity = 0; 
            state = 6; 
          }
          break; 
        case 6: 
          // color show 
          buttonState = digitalRead(button); 
          showLED = encoderPos; 
          if (showLED < 1) { showLED = 1; } 
          finish = start + showLED; 
          if (start < 0) {dir = 1; }
          if (finish > 24) {dir = -1; }
          for (int i = 0; i < start; i++) { 
            strip.setPixelColor(i, 0, 0, 0);
          }
          for (int i = start; i < finish; i++) { 
            strip.setPixelColor(i, red, green, blue);
          }
          for (int i = finish; i < 24; i++) { 
            strip.setPixelColor(i, 0, 0, 0);
          }
          strip.show();
          if (dir == 1) { 
            start++; 
            finish++; 
          } else if (dir == -1) { 
            start--; 
            finish--; 
          }
          delay(stepTime);
          
          if (buttonState == LOW) { 
            blue = intensity; 
            state = 7; 
          } 
          break; 
        case 7: 
          // button debounce state 
          buttonState = digitalRead(button); 
          if (buttonState == HIGH) { 
            encoderPosition = 0; 
            encoderPos = 0; 
            intensity = 0; 
            state = 0; 
          }
          break; 
        default: 
            break; 
    }
    
} 

void doEncoderA(){
  if( digitalRead(encoderA) != A_set ) {  // debounce once more
    A_set = !A_set;
    // adjust counter + if A leads B
    if ( A_set && !B_set ) {
      encoderPos += 1;
      if (encoderPos > 24) {
          encoderPos = 24; 
      }
    }
  }
}

// Interrupt on B changing state, same as A above
void doEncoderB(){
   if( digitalRead(encoderB) != B_set ) {
    B_set = !B_set;
    //  adjust counter - 1 if B leads A
    if( B_set && !A_set ) {
      encoderPos -= 1;
      if (encoderPos < 0) { 
          encoderPos = 0; 
      }
    }
  }
}



#define USE_USBCON  // NEEDED FOR ATmega32u4 - FEATHER OR LEONARDO!!
#include <ros.h>
#include <std_msgs/Empty.h>
#include <std_msgs/String.h>
#include <std_msgs/UInt16.h>

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN 6
#define NUMBER_OF_LEDS_IN_STRIP  67

#define LEDS_IN_UPPER_ARM  38
#define LEDS_IN_LOWER_ARM  (15+14)
#define POSITIONS_IN_ARM  (15+LEDS_IN_UPPER_ARM)

// CONSTANTS
const int BRIGHTNESS =  5;
const int WHITE_LEVEL = 10;
const int RED_LEVEL =   BRIGHTNESS;
const int GREEN_LEVEL = BRIGHTNESS;
const int BLUE_LEVEL =  BRIGHTNESS;
const int DEBUG_STRING_LEN    = 39;

// GLOBALS
ros::NodeHandle nh;
long            randNumber;
int             colorMode;
String          debugString;
char            debugStringChar[40];

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMBER_OF_LEDS_IN_STRIP, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void messageCb(const std_msgs::UInt16& cmd_msg){

  colorMode = cmd_msg.data;
  debugString = String("ARM LED MODE = ") + String(colorMode);
  debugString.toCharArray(debugStringChar, DEBUG_STRING_LEN );
  nh.loginfo(debugStringChar);
 
}

ros::Subscriber<std_msgs::UInt16> sub("arm_led_mode", &messageCb);


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  // End of trinket special code

  colorMode = 0;

  pinMode(LED_BUILTIN, OUTPUT);
  // Serial.begin(57600);
  nh.initNode();
  nh.subscribe(sub);

  // blink LED on the board at startup
  for (int i = 0; i < 10; i++) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(50);
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);
  }

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  randomSeed(analogRead(0));
}

void loop() {

  digitalWrite(LED_BUILTIN, HIGH);

  //colorTest(strip.Color(50,50,50), 50); // White


  if(1 == colorMode) {
    BluePulse(WHITE_LEVEL, WHITE_LEVEL, WHITE_LEVEL, 5); // White with blue pulses
  }
  // SOLID COLORS
  else if(2 == colorMode) {
    colorWipe(strip.Color(RED_LEVEL, 0, 0), 0); // Solid Red
  }
  else if(3 == colorMode) {
    colorWipe(strip.Color(0, GREEN_LEVEL, 0), 0); // Solid Green
  }
  else if(4 == colorMode) {
    colorWipe(strip.Color(0, 0, BLUE_LEVEL), 0); // Solid Blue
  }
  else if(5 == colorMode) {
    colorWipe(strip.Color(WHITE_LEVEL, WHITE_LEVEL, WHITE_LEVEL), 0); // Solid White
  }
  // STROBE COLORS
  else if(6 == colorMode) {
    colorTest(RED_LEVEL,0,0,10); // Strobe Red
  }
  else if(7 == colorMode) {
    colorTest(0,GREEN_LEVEL,0,10); // Strobe Green
  }
  else if(8 == colorMode) {
    colorTest(0,0,BLUE_LEVEL,10); // Strobe Blue
  }
  else if(9 == colorMode) {
    colorTest(WHITE_LEVEL, WHITE_LEVEL, WHITE_LEVEL, 10); // White
  }
  else if(10 == colorMode) {
    colorTest(WHITE_LEVEL, WHITE_LEVEL, WHITE_LEVEL, 10); // Bright White
  }
  // FAST COLORS
  else if(11 == colorMode) {
    fastColorFill(RED_LEVEL,0,0);         
  }
  else if(12 == colorMode) {
    fastColorFill(0,GREEN_LEVEL,0);        
  }
  else if(13 == colorMode) {
    fastColorFill(0,0,BLUE_LEVEL);         
  }
  else if(14 == colorMode) {
    fastColorFill(WHITE_LEVEL,WHITE_LEVEL,WHITE_LEVEL);         
  }

  else if(15 == colorMode) {
    fastColorFill(128,0,0);         
  }
  else if(16 == colorMode) {
    fastColorFill(0,128,0);        
  }
  else if(17 == colorMode) {
    fastColorFill(0,0,128);         
  }
  else if(18 == colorMode) {
    fastColorFill(128,128,128);         
  }

  else if(20 == colorMode) {
    rainbowCycle(10);         // Rainbow
  }

  else if(21 == colorMode) {  // SUPERBRIGHT
    fastColorFill(255,0,0);         
  }
  else if(22 == colorMode) {
    fastColorFill(0,255,0);        
  }
  else if(23 == colorMode) {
    fastColorFill(0,0,255);         
  }
  else if(24 == colorMode) {
    fastColorFill(255,255,255);         
  }
  
  
  else
  {
    // 0 or undefined. Turn LEDs OFF
    colorWipe(strip.Color(0, 0, 0), 0); // All Off
  }
 
  randNumber = random(10, 120);

  for (int i=0; i<randNumber; i++) {
    nh.spinOnce();
    delay(10);
  }
  //delay(randNumber);
  /*
    Pulse(RED_LEVEL, 0, 0, 10); // Red
    delay(1000);
    Pulse(0, GREEN_LEVEL, 0, 10); // Green
    delay(1000);
    Pulse(0, 0, BLUE_LEVEL, 10); // Blue
    delay(1000);
  */

  /****
    //colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
    // Send a theater pixel chase in...
    theaterChase(strip.Color(127, 127, 127), 50); // White
    theaterChase(strip.Color(127, 0, 0), 50); // Red
    theaterChase(strip.Color(0, 0, 127), 50); // Blue

    rainbow(20);
    rainbowCycle(20);
    theaterChaseRainbow(50);
  ***/
}


void setArmPixelColor(uint8_t armPosition, uint8_t r, uint8_t g, uint8_t b) {
  // convert pixel locations to match arm layout
  if ( armPosition >= POSITIONS_IN_ARM )
    return;

  if (armPosition < (LEDS_IN_UPPER_ARM)) { // upper arm, wired backward
    strip.setPixelColor((NUMBER_OF_LEDS_IN_STRIP - 1 ) - armPosition,  r , g , b );
  }
  else     {
    // lower arm
    // set right and left strip at same time
    strip.setPixelColor(armPosition - LEDS_IN_UPPER_ARM,  r , g , b ); // first strip of 15 leds
    if (armPosition < (POSITIONS_IN_ARM - 1)) {
      // handle that this strip only has 14 leds
      strip.setPixelColor((armPosition - LEDS_IN_UPPER_ARM) + 15,  r , g , b ); // second strip of 14 leds
    }
  }

}


void BluePulse(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  uint8_t intensity = 0;
  const uint8_t BRIGHTNESS_STEP = 5;
  const uint8_t PULSE_LEN = 6;
  const uint8_t FULL_BRIGHTNESS = (PULSE_LEN) * BRIGHTNESS_STEP;

  for (uint8_t i = 0; i < (POSITIONS_IN_ARM + PULSE_LEN); i++) {
    setArmPixelColor(i, r, g, b); // background color

    for (uint8_t pos = 0; pos <= PULSE_LEN; pos++) {
      intensity = FULL_BRIGHTNESS - (pos * BRIGHTNESS_STEP);

      if ((i - pos) >= 0) {
        if ( intensity > 0 )
          setArmPixelColor(i - pos, r, g, (b * intensity));
        else
          setArmPixelColor(i - pos, r, g, b);

      }
    }
    strip.show();
    nh.spinOnce();
    delay(wait);
  }

}

// //////////////////////////////////////////////
void colorTest(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {

  colorWipe(strip.Color(0, 0, 0), 0); // off

  // Delay before lighting leds again
  for (int i=0; i<50; i++) {
    nh.spinOnce();
    delay(10);
  }


  for (uint16_t i = 0; i < POSITIONS_IN_ARM; i++) {
    setArmPixelColor(i, r, g, b);
    strip.show();
    nh.spinOnce();
    delay(wait);
  }
}

// //////////////////////////////////////////////
void fastColorFill(uint8_t r, uint8_t g, uint8_t b) {

  for (uint16_t i = 0; i < POSITIONS_IN_ARM; i++) {
    setArmPixelColor(i, r, g, b);
    strip.show();
  }
}

// PULSE
void Pulse(uint8_t r, uint8_t g, uint8_t b, uint8_t wait) {
  uint8_t intensity = 0;
  const uint8_t BRIGHTNESS_STEP = 5;

  for (uint16_t i = 0; i < (strip.numPixels() - 5); i++) {
    for (uint16_t pos = 0; pos < 5; pos++) {
      intensity = (pos + 1) * BRIGHTNESS_STEP;
      strip.setPixelColor(i + pos, r * intensity, g * intensity, b * intensity);
    }
    strip.show();
    nh.spinOnce();
    delay(wait);
  }

}




// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}


void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
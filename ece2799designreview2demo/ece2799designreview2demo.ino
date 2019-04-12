#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FastLED.h>

#define OLED_RESET 4
#define LED_PIN 5
#define NUM_LEDS 1
#define BRIGHTNESS 64
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define THERMISTORPIN A0
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 10
#define BCOEFFICIENT 3977
#define SERIESRESISTOR 10000
#define IDEALTEMP 57.8


Adafruit_SSD1306 display(OLED_RESET);
CRGB leds[NUM_LEDS];

uint16_t samples[NUMSAMPLES];


void setup() {
  // put your setup code here, to run once:
  delay(1000); //boot up safely
//  Serial.begin(9600);
  analogReference(EXTERNAL);

  //setup LED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip ); //add leds
  FastLED.setBrightness(BRIGHTNESS); //setup brightness
  FastLED.show(); //update the LEDs

  //setup display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //setup display with appropriate I2C address
   display.clearDisplay(); //clear the display
  display.setTextSize(1); //set the text size
  display.setTextColor(WHITE); //set the text color to "white" (just turn pixel on in our case)
  display.setCursor(0,0); //set cursor in top left corner
  display.println("Setup"); //print text
  display.display(); //upadte the display
  delay(1000); //wait a second
  display.clearDisplay(); //clear the display
  
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t i;
  float average;

   // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++) {
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }

  // average all the samples out
  average = 0;
  for (i = 0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
  Serial.print("Average analog reading "); 
  Serial.println(average);
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);

  //example temp reading code
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C

  //set LED to appropriate color
  if(steinhart > IDEALTEMP + 10){
    leds[0] = CRGB(255, 0, 0);
  }
  else if(steinhart < IDEALTEMP + 10 && steinhart > IDEALTEMP - 10){
    leds[0] = CRGB(0, 255, 0);
  }
  else if(steinhart < IDEALTEMP - 10){
    leds[0] = CRGB(0, 0, 255);
  }
  FastLED.show();

  //print out temperatures, via serial and display
//  Serial.print("Temperature "); 
//  Serial.print(steinhart);
//  Serial.println(" *C");
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("ECE2799 Demo");
  display.setCursor(0,16);
  display.print("Temp: ");
  display.print(steinhart);
  display.print(" C");
  display.setCursor(0,32);
  display.print("Ideal: ");
  display.print(IDEALTEMP);
  display.print(" C");
  display.display();

  //delay every half a second
  delay(500);
}

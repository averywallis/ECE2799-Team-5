/*
 * Code used for design review #2 demonstration
 * 
 * Reads thermistor circuit output and display corresponding temperature
 * 
 * Shorted pot used to simulate thermistor
 * 
 * Wiring connections and circuit descriptions:
 * Pin connection format: Circuit pins - Arduino
 * 
 * 
 * Display:
 * VCC - 5V
 * GND - GND
 * SCL - A5
 * SDA - A4
 * 
 * 
 * LED:
 * 220 resistor between data in pin and Arduino pin
 * VCC - 5v
 * GND - GND
 * Din - D5
 * 
 * 'Thermistor' circuit:
 * Acts as voltage divider circuit
 * R1 = 10kOhms
 * R2 = 'Thermistor' (really 10k shorted potentiometer to act as variable resistor)
 * 
 * VCC - 3.3V
 * GND - GND
 * Vout - A0
 * 
 * 
 * 
 */

//appropriate libraries
#include <Adafruit_GFX.h> //display library
#include <Adafruit_SSD1306.h> //display library
#include <FastLED.h> //addressable LED library

//macros used for display, LED, thermistor calculations
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


Adafruit_SSD1306 display(OLED_RESET); //instance of display object
CRGB leds[NUM_LEDS]; //instance of LED object

uint16_t samples[NUMSAMPLES]; //array of samples


void setup(){
  // put your setup code here, to run once:
  delay(1000); //boot up safely (not entirely required, but here because why not)
  Serial.begin(9600); //setup communications between Arduino and computer
  analogReference(EXTERNAL); //setup reference for ADC as external (3.3V in this case)

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
  Serial.print("Average analog reading "); //print serially to computer
  Serial.println(average); //send to computer
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  Serial.print("Thermistor resistance "); 
  Serial.println(average);

  //example temp reading code, using simplified Steinhart-hart equation
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C

  //set LED to appropriate color
  if(steinhart > IDEALTEMP + 10){ //if hotter than ideal + 10
    leds[0] = CRGB(255, 0, 0); //set to red
  }
  else if(steinhart < IDEALTEMP + 10 && steinhart > IDEALTEMP - 10){ //if +/- 10 from ideal
    leds[0] = CRGB(0, 255, 0); //set to green
  }
  else if(steinhart < IDEALTEMP - 10){ //if colder than ideal - 10
    leds[0] = CRGB(0, 0, 255); //set to blue
  }
  FastLED.show();

  //print out temperatures, via serial and display
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");

  //display info on OLED
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

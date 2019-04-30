//include appropriate libraries
#include <Adafruit_GFX.h> //display library
#include <Adafruit_SSD1306.h> //display library
#include <FastLED.h> //addressable LED library
#include "LowPower.h" //low power library

//macros used for display, LED, thermistor calculations
#define OLED_RESET 4
#define LED_PIN 5
#define NUM_LEDS 1
#define BRIGHTNESS 64
#define LED_TYPE WS2812B
#define COLOR_ORDER GRB
#define THERMISTORPIN A7
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 10
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000
#define IDEALTEMP 57.8


Adafruit_SSD1306 display(OLED_RESET); //instance of display object
CRGB leds[NUM_LEDS]; //instance of LED object

uint16_t samples[NUMSAMPLES]; //array of samples

//array of time to cool numbers 
int time_to_cool[] = {0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0,
                         0,0,0,0,0,0,0,0,0,0};


void setup(){
  // put your setup code here, to run once:
  delay(1000); //boot up safely (not entirely required, but here because why not)
  Serial.begin(9600); //setup serial communications between Arduino and computer
  analogReference(EXTERNAL); //setup reference for ADC as external (3.3V in this case)

  //setup LED
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip ); //add leds
  FastLED.setBrightness(BRIGHTNESS); //setup brightness
  FastLED.show(); //update the LEDs

  //setup display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //setup display with appropriate I2C address (0x3C in our case)
  display.clearDisplay(); //clear the display
  display.setTextSize(1); //set the text size
  display.setTextColor(WHITE); //set the text color to "white" (just turn pixel on in our case)
  display.setCursor(0,0); //set cursor in top left corner
  display.println("Setup"); //print text
  display.display(); //upadte the display
  delay(1000); //wait a second
  display.clearDisplay(); //clear the display

  for(int a = 0; a < 58; a++){ //for each of the first 57 entries in the array
    time_to_cool[a] = 0; //set them equal to 0
  }
  //set the rest to appropriate values
  time_to_cool[58] = 51;
  time_to_cool[59] = 110;
  time_to_cool[60] = 153;
  time_to_cool[61] = 198;
  time_to_cool[62] = 238;
  time_to_cool[63] = 282;
  time_to_cool[64] = 318;
  time_to_cool[65] = 357;
  time_to_cool[66] = 386;
  time_to_cool[67] = 426;
  time_to_cool[68] = 461;
  time_to_cool[69] = 489;
  time_to_cool[70] = 518;
  time_to_cool[71] = 552;
  time_to_cool[72] = 580;
  time_to_cool[73] = 610;
  time_to_cool[74] = 638;
  time_to_cool[75] = 663;
  time_to_cool[76] = 692;
  time_to_cool[77] = 713;
  time_to_cool[78] = 741;
  time_to_cool[79] = 772;
  time_to_cool[80] = 801;
  time_to_cool[81] = 831;
  time_to_cool[82] = 848;
  time_to_cool[83] = 872;
  time_to_cool[84] = 895;
  time_to_cool[85] = 919;
  time_to_cool[86] = 945;
  time_to_cool[87] = 970;
  time_to_cool[88] = 1000;
  time_to_cool[89] = 1028;
  time_to_cool[90] = 1048;
  time_to_cool[91] = 1070;
  time_to_cool[92] = 1092;
  time_to_cool[93] = 1120;
  time_to_cool[94] = 1142;
  time_to_cool[95] = 1166;
  time_to_cool[96] = 1192;
  time_to_cool[97] = 1220;
  time_to_cool[98] = 1246;
  time_to_cool[99] = 1271;
  time_to_cool[100] = 1294;
//  Serial.print(time_to_cool[86]); //debugging that values actually worked
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //values used later in the code
  uint8_t i;
  float average;
  int temp = 0;
  float steinhart;
  int time1 = 0;




  for (i = 0; i < NUMSAMPLES; i++){ //for the number of samples
   samples[i] = analogRead(THERMISTORPIN); //get a reading from the thermistor and store it in the array
   delay(10); //slight delay for stability
  }

  // average all the samples out
  average = 0; //set average to 0
  for (i = 0; i< NUMSAMPLES; i++){ //for each of the samples
     average += samples[i]; //add that to the average
  }
  average /= NUMSAMPLES; //calculate the average

  //for debugging
//  Serial.print("Average analog reading "); //print serially to computer
//  Serial.println(average); //send to computer
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
//  Serial.print("Thermistor resistance "); 
//  Serial.println(average); //for debugging

  //Calculate the temperature from the calculated resistance
  //using simplified Steinhart-hart equation:
  // 1/T = 1/T0 + 1/B * ln(R/Ro)
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert to get temp in Kelvin
  steinhart -= 273.15;                         // convert to Celsius
  temp = steinhart;

//  time1 = time_to_cool[0]; //attempting to use lookup table, but for whatever reason won't execute properly
// potentially variable outside scope, but that should throw a compiler error, which this doesn't
  time1 = 30 * (temp - 57) / 60; //use a simpliefied equation to calculate the time till the drink is at ideal



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
  FastLED.show(); //update the LED

  //print out temperatures, via serial and display
  Serial.print("Temperature "); 
  Serial.print(steinhart);
  Serial.println(" *C");

  //display info on OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("ECE2799 Team 5");
  display.print("Temp: ");
  display.print(steinhart);
  display.println(" C");
  display.print("Ideal: ");
  display.print(IDEALTEMP);
  display.println(" C");
  display.println("Time till ideal: ");

  if(time1 > -1){ //if we got a positive number (ie still time to cool)
    display.print(time1);
    display.print(" minutes");
  }
  else{ //else we've passed the ideal temperature
    display.print("passed ideal");
  }

  display.display(); //update the display

  //low power mode for 4 seconds
  LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);  
  display.clearDisplay(); //clear the display to prevent burn-in
  display.display(); //flush to the screen
  
}

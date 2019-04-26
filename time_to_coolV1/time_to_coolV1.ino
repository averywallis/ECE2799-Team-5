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

//macros used for display, LED, thermistor calculations
#define OLED_RESET 4
#define THERMISTORPIN A7
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 10
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000
#define IDEALTEMP 57.8


Adafruit_SSD1306 display(OLED_RESET); //instance of display object

uint16_t samples[NUMSAMPLES]; //array of samples
float T0 = 0;
float last_temp = 0;
long unsigned int new_time = 0;
long unsigned int old_time = 0;


void setup(){
  // put your setup code here, to run once:
  delay(1000); //boot up safely (not entirely required, but here because why not)
  Serial.begin(9600); //setup communications between Arduino and computer
  analogReference(EXTERNAL); //setup reference for ADC as external (3.3V in this case)


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

  delay(10000); //wait 10 seconds to get T0 reading

  //get readings
  for (int i=0; i< NUMSAMPLES; i++){
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }
  
  // average all the samples out
  float average0 = 0;
  for (int i = 0; i< NUMSAMPLES; i++) {
     average0 += samples[i];
  }
  average0 /= NUMSAMPLES;
  
  average0 = 1023 / average0 - 1;   // convert the value to resistance
  average0 = SERIESRESISTOR / average0; //get the resistance of the resistor
  float steinhart0;
  steinhart0 = average0 / THERMISTORNOMINAL;     // (R/Ro)
  steinhart0 = log(steinhart0);                  // ln(R/Ro)
  steinhart0 /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart0 += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart0 = 1.0 / steinhart0;                 // Invert
  steinhart0 -= 273.15;                         // convert to C
  T0 = steinhart0; //save original temp T(0)
  last_temp = T0; //set this temp as the last_temp
  old_time = millis(); //save the current time
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t i;
  float average;
  float m = 0;
  float time_to_cool = 0;

   // take N samples in a row, with a slight delay
  for (i=0; i< NUMSAMPLES; i++){
   samples[i] = analogRead(THERMISTORPIN);
   delay(10);
  }

  // average all the samples out
  average = 0;
  for (i = 0; i< NUMSAMPLES; i++) {
     average += samples[i];
  }
  average /= NUMSAMPLES;
//  Serial.print("Average analog reading "); //print serially to computer
//  Serial.println(average); //send to computer
  
  average = 1023 / average - 1;   // convert the value to resistance
  average = SERIESRESISTOR / average; //get the resistance of the resistor
//  Serial.print("Thermistor resistance "); 
//  Serial.println(average);

  //example temp reading code, using simplified Steinhart-hart equation
  float steinhart;
  steinhart = average / THERMISTORNOMINAL;     // (R/Ro)
  steinhart = log(steinhart);                  // ln(R/Ro)
  steinhart /= BCOEFFICIENT;                   // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;                 // Invert
  steinhart -= 273.15;                         // convert to C


  //need verification that this is a correct way to do it
  //using y = mx + b, where is y is a temp as a function of time, m = change in temp since last reading
  //b = t(0), or initial reading. may want to use previous time
  new_time = millis(); //get new time
  m = (steinhart - last_temp) / (new_time - old_time); //calculate the new slope
  time_to_cool = (IDEALTEMP - T0) / m; //get the estimated time to cool
  old_time = new_time; //save the old time

  //display info on OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.setCursor(0,16);
  display.print("Temp: ");
  display.print(steinhart);
  display.print(" C");
  display.setCursor(0,32);
  display.print("Time to cool: ");
  display.print(IDEALTEMP);
  display.print(" seconds");
  display.display();

  

  //delay every half a second
  delay(250);
}

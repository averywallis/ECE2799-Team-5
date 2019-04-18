/*
 * Code used during our testing impedance of ADC data collect on April 12
 * Outputs the ADC code, the corresponding voltage, and the calculated thermistor resistor to the display
 * 
 * 
 * Pin connections and circuit setup:
 * Pin out connection format: Circuit pins - Arduino
 * 
 * Display:
 * VCC - 5V
 * GND - GND
 * SCL - A5
 * SDA - A4
 * 
 * Thermistor circuit:
 * Voltage divider circuit, with R1 = 10k and R2 = 'thermistor'
 * Thermistor is replaced with a shorted 10k ohm resistor to simulate a thermistor
 * VCC - 3.3V
 * GND - GND
 * Vout - A0
 * 
 * 
 */

//libraries for display
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//macros
#define OLED_RESET 4
#define REF 3.3
#define NUMSAMPLES 10
#define THERMISTORPIN A7
#define SERIESRESISTOR 10000

Adafruit_SSD1306 display(OLED_RESET); //instance of display object

uint16_t samples[NUMSAMPLES]; //array of samples
float resistance = 0; //var for calculated resistance

void setup() {
  // put your setup code here, to run once:
  delay(1000); //boot safely
  analogReference(EXTERNAL);


  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //setup display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Setup");
  display.display();
  delay(2000);
  display.clearDisplay();
  display.display();

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
  average /= NUMSAMPLES; //get average of samples
  
  float voltage = average*(REF / 1023.0); //get voltage value of code value
  resistance = 1023 / average - 1;
  resistance = SERIESRESISTOR / resistance; //convert to resistance
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Code: ");
  display.println(average);
  display.print("Voltage: ");
  display.println(voltage,3);
  display.print("Resistance: ");
  display.print(resistance);
  
  display.display();
  delay(250);
  
}

//testing difference

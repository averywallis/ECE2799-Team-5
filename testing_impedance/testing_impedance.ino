#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
#define REF 3.3
#define NUMSAMPLES 10
#define THERMISTORPIN A0
#define SERIESRESISTOR 10000

Adafruit_SSD1306 display(OLED_RESET);

uint16_t samples[NUMSAMPLES];
float resistance = 0;

void setup() {
  // put your setup code here, to run once:
  delay(1000);
  analogReference(EXTERNAL);


  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
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
  average /= NUMSAMPLES;
  
  float voltage = average*(REF / 1023.0);
  resistance = 1023 / average - 1;
  resistance = SERIESRESISTOR / resistance;
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Code: ");
  display.println(average);
  display.print("Voltage: ");
  display.println(voltage);
  display.print("Resistance: ");
  display.print(resistance);
  
  display.display();
  delay(250);
  
}

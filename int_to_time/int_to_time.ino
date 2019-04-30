#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4

Adafruit_SSD1306 display(OLED_RESET); //instance of display object


void setup() {
  // put your setup code here, to run once:
  delay(1000); //boot up safely (not entirely required, but here because why not)

  //setup display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); //setup display with appropriate I2C address
  display.clearDisplay(); //clear the display
  display.setTextSize(1); //set the text size
  display.setTextColor(WHITE); //set the text color to "white" (just turn pixel on in our case)
  display.setCursor(0,0); //set cursor in top left corner
  display.println("Setup"); //print text
  display.display(); //upadte the display
  delay(1000); //wait a second
  display.clearDisplay(); //clear the 
}

void loop() {
  // put your main code here, to run repeatedly:

  float minutes = 0;
  int elapsed = 0;

  elapsed = 500;

  minutes = (float)elapsed / 60;
  display.setCursor(0,0); //set cursor in top left corner
  display.print(minutes);
  display.print(" Minutes");
  display.display();
  delay(1000);

  
  display.clearDisplay();
  display.display();

}

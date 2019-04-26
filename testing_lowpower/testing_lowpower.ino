// **** INCLUDES *****
#include "LowPower.h" //low power library by 

int led = 2;

void setup()
{
    // No setup is required for this library
    pinMode(13, OUTPUT);
    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    for( char a = 0; a <3; a++){
      delay(100);
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
    }

}

void loop() 
{
    //flash once to signal idle/sleep
    for( char a = 0; a <1; a++){
      delay(100);
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
    }
    

    // ATmega328P, ATmega168
//    LowPower.idle(SLEEP_8S, ADC_OFF, TIMER2_OFF, TIMER1_OFF, TIMER0_OFF, 
//                SPI_OFF, USART0_OFF, TWI_OFF);

    // Enter power down state for 8 s with ADC and BOD module disabled
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  

    
  //flash 3 times to signal normal operation
    for( char a = 0; a <3; a++){
      delay(100);
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
    }
    delay(8000);
}

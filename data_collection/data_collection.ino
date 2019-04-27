#define THERMISTORPIN A7
#define THERMISTORNOMINAL 10000
#define TEMPERATURENOMINAL 25
#define NUMSAMPLES 10
#define BCOEFFICIENT 3950
#define SERIESRESISTOR 10000
#define IDEALTEMP 57.8

uint16_t samples[NUMSAMPLES]; //array of samples
//unsigned char data[10000]; //array of data
unsigned int sample_num = 0;


void setup(){
  // put your setup code here, to run once:
  delay(1000); //boot up safely (not entirely required, but here because why not)
  Serial.begin(9600); //setup communications between Arduino and computer
  analogReference(EXTERNAL); //setup reference for ADC as external (3.3V in this case)


    //Section for setting up the extension/data collection with Excel
    //Serial.println("CLEARDATA"); // clears sheet starting at row 2
    Serial.println("CLEARSHEET"); // clears sheet starting at row 1
    
  // define 5 columns named "Date", "Time", "Temperature", "Millis"
    Serial.println("LABEL,Date,Time,Temp,Millis");


  //don't know what this does
  // set the names for the 3 checkboxes
    Serial.println("CUSTOMBOX1,LABEL,Stop logging at 250?");
    Serial.println("CUSTOMBOX2,LABEL,Resume log at 350?");
    Serial.println("CUSTOMBOX3,LABEL,Quit at 450?");

  // check 2 of the 3 checkboxes (first two to true, third to false)
    Serial.println("CUSTOMBOX1,SET,0");
    Serial.println("CUSTOMBOX2,SET,0");
    Serial.println("CUSTOMBOX3,SET,0");
  
}

void loop(){
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
//  Serial.print("Average analog reading "); //print serially to computer
//  Serial.println(average); //send to computer
  
  // convert the value to resistance
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
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


//  data[sample_num] = (int)floor(steinhart); //save the int version of the temp reading
//  sample_num++; //increment the sample number count
  
//  //print out temperatures, via serial and display
//  Serial.print("Temperature "); 
//  Serial.print(steinhart);
//  Serial.println(" *C");

  //print out in format that is usable by excel writer
  Serial.print("DATA,DATE,TIME,");
  Serial.print(steinhart);
  Serial.print(",");
  Serial.println(millis());

  delay(1000); //delay for 1 seconds
  
}

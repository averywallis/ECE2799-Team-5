int a = 0;
int b = 0;

void setup() {
  // put your setup code here, to run once:
  // open serial connection
    Serial.begin(9600);

    //Serial.println("CLEARDATA"); // clears sheet starting at row 2
    Serial.println("CLEARSHEET"); // clears sheet starting at row 1
    
  // define 5 columns named "Date", "Time", "Timer", "Counter" and "millis"
    Serial.println("LABEL,Date,Time,Data,Date,Time");


  //don't know what this does
  // set the names for the 3 checkboxes
    Serial.println("CUSTOMBOX1,LABEL,Stop logging at 250?");
    Serial.println("CUSTOMBOX2,LABEL,Resume log at 350?");
    Serial.println("CUSTOMBOX3,LABEL,Quit at 450?");

  // check 2 of the 3 checkboxes (first two to true, third to false)
    Serial.println("CUSTOMBOX1,SET,1");
    Serial.println("CUSTOMBOX2,SET,1");
    Serial.println("CUSTOMBOX3,SET,0");
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.print("DATA,DATE,TIME,");
  Serial.print(a);
  Serial.print(",");
  Serial.print(millis());
  Serial.print(",");
  Serial.println(b);
  a++;
  b += 10;
  delay(100);

}

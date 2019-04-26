unsigned long x1;

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.print("Time: ");
  x1 = millis();

  Serial.println(x1); //prints time since program started
  delay(1000);          // wait a second so as not to send massive amounts of data
}

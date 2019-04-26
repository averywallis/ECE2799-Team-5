
#define NUM_DATA 10

unsigned char Xs[NUM_DATA] = {6,6,9,3,3,10,8,6,3,5};
unsigned char Ys[NUM_DATA] = {8,4,5,9,7,5,8,3,4,2};

float m = 0;
float b = 0;  

boolean x = true;

unsigned long t1;
unsigned long t2;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(x){

    float xbar = 0;
    float ybar = 0;
    float sum_numerator = 0;
    float sum_denomenator = 0;

    float t = 0;
    t1 = micros();
    Serial.println(t1);
  
    //get mean of Xs and Ys
    for(unsigned char index = 0; index < NUM_DATA; index++){
      //sum them up
      xbar += (float)Xs[index];
      ybar += (float)Ys[index];
    }
    //divide by number of samples to get mean
    xbar /= NUM_DATA;
    ybar /= NUM_DATA;

    //get the numerator and denominator of slops
    for(unsigned char index = 0; index < NUM_DATA; index++){
      sum_numerator += (Xs[index] - xbar) * (Ys[index] - ybar);
      sum_denomenator += ( (Xs[index] - xbar) * (Xs[index] - xbar));
    }
    m = sum_numerator / sum_denomenator;

    //calculate the y-intercept of the line
    b = ybar - m * xbar;

    t2 = micros();
    Serial.println(t2);
    t = floor((t2 - t1));
    
    Serial.print("Time elapsed (microseconds): ");
    Serial.println(t);
    
    Serial.print("m: ");
    Serial.println(m);
    Serial.print("b: 0");
    Serial.print(b);
    x = false;
//    delay(500); //delay if we want to print repeatedly
  }


}

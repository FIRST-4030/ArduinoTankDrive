int ch1;
int ch2;
float output1 = 0;
float output2 = 0;
float inputMax;
float inputMin;
float inputScaler;
float inputMedian;

#define RangeMax 10000
#define RangeMin 0
//channel 3, 
#define LeftYPin 35
//channel 1
#define RightXPin 45


void setup() {
  // put your setup code here, to run once:

// Set our input pins as such
pinMode(LeftYPin, INPUT); 
pinMode(RightXPin, INPUT); 

inputMax = RangeMin;
inputMin = RangeMax;

Serial.begin(9600); 
}



void loop() {
  while(true){

  //set motor speeds
  analogWrite(55, output1);
  analogWrite(56, output2);

  // Read the pulse width of each channel
  ch1 = pulseIn(LeftYPin, HIGH, 25000); 
  ch2 = pulseIn(RightXPin, HIGH, 25000); 
  //Serial.print("ch 1: ");  
  //Serial.println(ch1);
  //Serial.print("ch 2: ");
  //Serial.println(ch2);
  
  if ((ch1 == 0) || (ch2 == 0))
    {//stop now 
    output1 = 0;
    output2 = 0;
    continue; 
    }
  else 
    {
    //check min max
     if (ch1 > inputMax) 
      {
        inputMax = ch1;
      }
     else if (ch1 < inputMin) 
      {
        inputMin = ch1;
      }

      //Serial.print("inputmax 1: ");  
      //Serial.println(inputMax);
      //Serial.print("inputmin 2: ");
      //Serial.println(inputMin);
        
      inputScaler = ((inputMax - inputMin)/2);
      inputMedian = (inputScaler + inputMin);
      //Serial.print("inputscaler 1: ");  
      //Serial.println(inputScaler);
      //Serial.print("inputmedian 2: ");
      //Serial.println(inputMedian);
    }

  //convert to range of -1 to 1
  output1 = (ch1 - inputMedian)/inputScaler;
  output2 = (ch2 - inputMedian)/inputScaler;
  
  //limit output values to range
  if (output1 > 1)
    {
    output1 = 1;
    }
  if (output1 < -1)
    { 
    output1 = -1;
    }
  if (output2 > 1)
    {
    output2 = 1;
    }
  if (output2 < -1)
    { 
    output2 = -1;
    }

  //make the center 0 point slightly larger
  if ((output1 < 0.1) && (output1 > -0.1))
    {
    output1 = 0;
    }
  if ((output2 < 0.1) && (output2 > -0.1))
    { 
    output2 = 0;
    }

  Serial.print("Output 1: ");  
  Serial.println(output1);
  Serial.print("Output 2: ");
  Serial.println(output2);
  
  //convert to output range of 0 to 255
  output1 = ((output1 * 127.5) + 127.5);
  output2 = ((output2 * 127.5) + 127.5);

  Serial.print("Output 3: ");  
  Serial.println(output1);
  Serial.print("Output 4: ");
  Serial.println(output2);
  
  delay(1000);
  }
}




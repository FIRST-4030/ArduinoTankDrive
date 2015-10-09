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
#define LeftYPin 43
//channel 1
#define RightXPin 53


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

  if ((ch1 = 0) || (ch2 = 0))
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

      inputScaler = ((inputMax - inputMin)/2);
      inputMedian = (inputScaler + inputMin);
    }

  output1 = (ch1 - inputMedian)/inputScaler;
  output2 = (ch2 - inputMedian)/inputScaler;
  
  Serial.print("Output value: ");  // Print the value of each channel
  Serial.println(ch1);
  Serial.println(ch2);
  }
}




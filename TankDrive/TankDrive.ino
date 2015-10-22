#define STOP 0
#define SERIAL_SPEED 9600
#define rangeMax 5000
#define rangeMin 500
// Receiver hannel 3
#define LeftYPin 35
// Receiver channel 1
#define RightXPin 45
#define OutputScaler (255/2)
#define DeadZone 0.05

int ch1;
int ch2;
float output1 = 0;
float output2 = 0;
float inputMax;
float inputMin;
float inputScaler;
float inputMedian;

void setup();
void loop();
void CheckMinMax(float input);

void setup() {
  // Set our input pins as such
  pinMode(LeftYPin, INPUT); 
  pinMode(RightXPin, INPUT); 

  inputMax = rangeMin;
  inputMin = rangeMax;
  output1 = STOP;
  output2 = STOP;

  Serial.begin(SERIAL_SPEED); 
}

void loop() {
  while(true){

  //set motor speeds
  Serial.print("Output: ");  
  Serial.print(output1);
  Serial.print(",");
  Serial.print(output2);
  analogWrite(55, output1);
  analogWrite(56, output2);

  // Read the pulse width of each channel
  ch1 = pulseIn(LeftYPin, HIGH, 25000); 
  ch2 = pulseIn(RightXPin, HIGH, 25000); 

  if ((ch1 <= rangeMin) || (ch2 <= rangeMin) || (ch1 >= rangeMax) || (ch2 >= rangeMax)){
    //stop now 
    output1 = 0;
    output2 = 0;
    continue; 

    //check min max
    CheckMinMax(ch1);
    CheckMinMax(ch2);
        
    inputScaler = ((inputMax - inputMin)/2);
    inputMedian = (inputScaler + inputMin);
    }

    
  //convert to range of -1 to 1
  output1 = (ch1 - inputMedian)/inputScaler;
  output2 = (ch2 - inputMedian)/inputScaler;
  
  //limit output values to range
  if (abs(output1) > 1){
     if (output1 < 0){
      output1 = -1;
     }
     else{ 
      output1 = 1;
     }
   }
  if (abs(output2) > 1){
     if (output2 < 0){
      output2 = -1;
     }
     else {
      output2 = 1;
     }
   }
  
    //make the center 0 point slightly larger
  if ((output1 < DeadZone) && (output1 > -DeadZone))
    {
    output1 = 0;
    }
  if ((output2 < DeadZone) && (output2 > -DeadZone))
    { 
    output2 = 0;
    }
    
  //convert to output range of 0 to 255
  output1 = ((output1 * OutputScaler) + OutputScaler);
  output2 = ((output2 * OutputScaler) + OutputScaler);

  Serial.print("Output 1: ");  
  Serial.println(output1);
  Serial.print("Output 2: ");
  Serial.println(output2);
  
  delay(1000);
  }
}

void CheckMinMax(float input){
     if (input > inputMax) 
      {
        inputMax = input;
      }
     else if (input < inputMin) 
      {
        inputMin = input;
      }
}

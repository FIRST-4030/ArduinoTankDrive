int ch1;
int ch2;
float output1;
float output2;


#define inputMax 2575
#define inputMin 1285
#define inputScaler ((inputMax - inputMin)/2)
#define inputMedian (inputScaler + inputMin)

//channel 3, 
#define LeftYPin 43
//channel 1
#define RightXPin 53


void setup() {
  // put your setup code here, to run once:

pinMode(LeftYPin, INPUT); // Set our input pins as such
pinMode(RightXPin, INPUT); // Set our input pins as such

Serial.begin(9600); // The author made a ridiculous pun. We don't know what this does.

}

void loop() {
  // put your main code here, to run repeatedly:

  ch1 = pulseIn(LeftYPin, HIGH, 25000); // Read the pulse width of each channel
  ch2 = pulseIn(RightXPin, HIGH, 25000); // Read the pulse width of each channel

  output1 = (ch1 - inputMedian); //inputScaler;
  output2 = (ch2 - inputMedian); //inputScaler;

  analogWrite(55, output1);
  analogWrite(56, output2);
  
  Serial.print("Output value: ");  // Print the value of each channel
  Serial.println(ch1);
  Serial.println(ch2);

}

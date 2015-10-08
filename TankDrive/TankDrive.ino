int ch1;
int ch2;
#define inputMax 2575
#define inputMin 1285
#define inputScaler ((inputMax - inputMin)/2)
#define inputMedian (scaler + inputMin)


void setup() {
  // put your setup code here, to run once:

pinMode(53, INPUT); // Set our input pins as such
pinMode(54, INPUT); // Set our input pins as such

Serial.begin(9600); // The author made a ridiculous pun. We don't know what this does.

}

void loop() {
  // put your main code here, to run repeatedly:

  ch1 = pulseIn(53, HIGH, 25000); // Read the pulse width of each channel
  ch2 = pulseIn(54, HIGH, 25000); // Read the pulse width of each channel
  
  //Serial.print("Current value: ");  // Print the value of each channel
  //Serial.println(ch1);

  delay(400); // Make the terminal window happier
}

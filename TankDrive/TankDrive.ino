#include <Servo.h>

// Comment this debug line for production runs
//#define DEBUG


	

#define IN_RANGE_MAX (4000)
#define IN_RANGE_MIN (500)
#define CALIBRATION_MAX (2000)
#define CALIBRATION_MIN (1300)
#define PIN_X (45) // Radio channel 1 (right X)
#define PIN_Y (35) // Radio channel 2 (right Y)
#define PIN_SPEED (40) //Radio channel ???? (left Y)
#define PWM_MIN (0)
#define PWM_MAX (180)
#define STOP ((int)(PWM_MAX - PWM_MIN) / 2)
#define SERIAL_SPEED (9600)
#define OUTPUT_SCALER ((int)(PWM_MAX - PWM_MIN) / 2)
#define DEAD_ZONE (10)
#define OUTPUT_LEFT (9)
#define OUTPUT_RIGHT (10)
#define READ_TIMEOUT (25000)
#define LEFT (0)
#define RIGHT (1)

// Globals
int inY;
int inX;
int inSpeed;
int outLeft = STOP;
int outRight = STOP;
int inputMax = IN_RANGE_MIN;
int inputMin = IN_RANGE_MAX;
float inputScaler;
float inputMedian;
float turnAtStop = 1;
float turnAtSpeed = 0;
Servo Motors[2];
boolean calibrationDone = false;

// Function prototypes
void setMinMax(int input);
int deadZone(int in);
float limitOutput(float in);
void setMotorSpeed(int motor, float in);
float speedLimit(float in);

void setup() {
 // Set our input pins as such
 pinMode(PIN_Y, INPUT);
 pinMode(PIN_X, INPUT);
 pinMode(PIN_SPEED, INPUT);

 //set output pins
 Motors[LEFT].attach(OUTPUT_LEFT);
 Motors[RIGHT].attach(OUTPUT_RIGHT);

 // Ensure our initial state is reasonable
 inputMax = IN_RANGE_MIN;
 inputMin = IN_RANGE_MAX;
 outLeft = STOP;
 outRight = STOP;

 // Pull up the serial port for debugging
 Serial.begin(SERIAL_SPEED);
}



void loop() {
  
  //init step - calibration and setting up pins
//main run loop 
	//read input	-outputs -1 to 1
	//tranformation
	//write output  -takes -1 to 1
	
while(true) {
 // Intermediate variables
 float tempX, tempY;
 float tempLeft, tempRight;


 
 setMotorSpeed(LEFT, outLeft);
 setMotorSpeed(RIGHT, outRight);

 //input and calibration
 readInputs()

 //speed limit




 
 #ifdef DEBUG
   delay(1000);
 #endif
}
}

void readInputs{
  
 // Read the pulse width of each channel
 inY = pulseIn(PIN_Y, HIGH, READ_TIMEOUT);
 inX = pulseIn(PIN_X, HIGH, READ_TIMEOUT);
 inSpeed = pulseIn(PIN_SPEED, HIGH, READ_TIMEOUT);
 
 Serial.print("Raw: ");
 Serial.print(inX);
 Serial.print(",");
 Serial.print(inY);
 Serial.print(",");
 Serial.println(inspeed);
 
 
 // On invalid input, stay in a tight loop
 if ((inY <= IN_RANGE_MIN) || (inX <= IN_RANGE_MIN) ||
   (inY >= IN_RANGE_MAX) || (inX >= IN_RANGE_MAX)) {
     outLeft = STOP;
     outRight = STOP;
     #ifdef DEBUG
       Serial.println("Invalid input");
       delay(1000);
     #endif
     continue;
 }

 // Calculate the input range
 setMinMax(inY);
 setMinMax(inX);

 // Don't consider the system ready until calibration is reasonable
 if ((inputMax <= CALIBRATION_MAX) || (inputMin >= CALIBRATION_MIN)) {
     outLeft = STOP;
     outRight = STOP;
     continue;
 }

 // Zero-center the input
 inY -= inputMedian;
 inX -= inputMedian;

 // Provide a small dead zone on the input data
 inY = deadZone(inY);
 inX = deadZone(inX);

 // Once calibration is valid, wait for a return to the dead zone
 if (!calibrationDone) {
   if (inX != 0 && inY != 0) {
     outLeft = STOP;
     outRight = STOP;
     continue;
   } else {
     calibrationDone = true;
   }
 }

 // Convert to range of -1 to 1
 tempY = (float)inY / inputScaler;
 tempX = (float)inX / inputScaler;
  
}


void setMinMax(int input){
 if (input > inputMax) {
   inputMax = input;
 } else if (input < inputMin) {
   inputMin = input;
 }
 inputScaler = (((float)inputMax - (float)inputMin) / 2.0);
 inputMedian = (inputScaler + (float)inputMin);
}

int deadZone(int in) {
 if (abs(in) < DEAD_ZONE) {
   in = 0;
 }
 return in;
}

float limitOutput(float in) {
 float out = in;
 if (abs(in) > 1){
   out = 1;
   if (in < 0) {
     out *= -1;
   }
 }
 return out;
}

float speedLimit(float in) {
   #ifdef DEBUG
   Serial.print("Scaled Input: ");
    Serial.print(tempX);
  Serial.print(",");
  Serial.println(tempY);
 #endif

 // Adjust values to tank drive
 tempLeft = tempY + tempX;
 tempRight = tempY - tempX;

 //more tunable version of tank drive - need to adjust values for quadrants
 //outLeft = inY + (turnAtStop * inX * (1 - inY));
 //outRight = inY - (turnAtStop * inX) + (inY * inX * (turnAtStop - turnAtSpeed - 1));

 // Limit output values to the valid range, in case our scaler is slightly off
 tempLeft = limitOutput(tempLeft);
 tempRight = limitOutput(tempRight);

 #ifdef DEBUG
   Serial.print("Scale Output: ");
  Serial.print(tempLeft);
  Serial.print(",");
  Serial.println(tempRight);
 #endif
  
  
}


void setMotorSpeed(int motor, float in) {
  if (speed != STOP){
    switch(motor){
      case LEFT:
        speed = (float)speed * 0.8;
        break;
      case RIGHT:
      default:
        break;
    }
  }

  // Scale to the output range
  outLeft = (int)((tempLeft * OUTPUT_SCALER) + OUTPUT_SCALER);
  outRight = (int(180)) - ((int)((tempRight * OUTPUT_SCALER) + OUTPUT_SCALER));
  
   // Set motor speeds
 Serial.print("Output: ");
 Serial.print(outLeft);
 Serial.print(",");
 Serial.println(outRight);

  //write motor speed
  Motors[motor].write(outLeft);
}

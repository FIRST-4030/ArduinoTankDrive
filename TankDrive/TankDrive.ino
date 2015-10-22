// Comment this debug line for production runs
#define DEBUG

#define STOP (0)
#define SERIAL_SPEED (9600)
#define IN_RANGE_MAX (5000)
#define IN_RANGE_MIN (500)
#define PIN_Y (35)
#define PIN_X (45)
#define OUTPUT_SCALER (255/2)
#define DEAD_ZONE (5)
#define OUTPUT_LEFT (55)
#define OUTPUT_RIGHT (56)
#define READ_TIMEOUT (25000)

// Globals
int inY;
int inX;
float outLeft = STOP;
float outRight = STOP;
float inputMax = IN_RANGE_MIN;
float inputMin = IN_RANGE_MAX;
float inputScaler;
float inputMedian;

// Function prototypes
void setMinMax(float input);
int deadZone(int in);
float limitOutput(float in);

void setup() {
  // Set our input pins as such
  pinMode(PIN_Y, INPUT); 
  pinMode(PIN_X, INPUT); 

  // Ensure our initial state is reasonable
  inputMax = IN_RANGE_MIN;
  inputMin = IN_RANGE_MAX;
  outLeft = STOP;
  outRight = STOP;

  // Pull up the serial port for debugging
  Serial.begin(SERIAL_SPEED); 
}

void loop() {
while(true) {

  // Set motor speeds
  Serial.print("Output: ");  
  Serial.print(outLeft);
  Serial.print(",");
  Serial.println(outRight);
  analogWrite(OUTPUT_RIGHT, outLeft);
  analogWrite(OUTPUT_LEFT, outRight);

  // Read the pulse width of each channel
  inY = pulseIn(PIN_Y, HIGH, READ_TIMEOUT); 
  inX = pulseIn(PIN_X, HIGH, READ_TIMEOUT);
  Serial.print("Raw: ");
  Serial.print(inY);
  Serial.print(",");
  Serial.println(inX);

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

  // Zero-center the input
  inY -= inputMedian;
  inX -= inputMedian;

  // Provide a small dead zone on the input data
  inY = deadZone(inY);
  inX = deadZone(inX);
  
  // Convert to range of -1 to 1
  outLeft = inY / inputScaler;
  outRight = inX / inputScaler;
  
  // Limit output values to the valid range, in case our scaler is slightly off
  outLeft = limitOutput(outLeft);
  outRight = limitOutput(outRight);
      
  // Scale to the output range
  outLeft = ((outLeft * OUTPUT_SCALER) + OUTPUT_SCALER);
  outRight = ((outRight * OUTPUT_SCALER) + OUTPUT_SCALER);

  #ifdef DEBUG
    delay(1000);
  #endif
}
}

void setMinMax(float input){
  if (input > inputMax) {
    inputMax = input;
  } else if (input < inputMin) {
    inputMin = input;
  }
  inputScaler = ((inputMax - inputMin) / 2);
  inputMedian = (inputScaler + inputMin);
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

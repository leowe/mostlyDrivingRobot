/*
 * Hapkit control skeleton code
 */

//Motor Pins
int pwmPinLeft = 3; // PWM output pin for motor 1
int dirPinLeft1 = 2; // direction output pin for motor 1
int dirPinLeft2 = 4; // direction output pin for motor 1
int pwmPinRight = 5;
int dirPinRight1 = 6;
int dirPinRight2 = 7;


void setup() {
  initializeMotor();

  //initialize encoder pins
}


//--------------------------------------------
// Main Loop
//--------------------------------------------
void loop() { 
  analogWrite(pwmPinLeft, 255); 
  analogWrite(pwmPinRight, 255);
}

//===========================================
//
// HELPER FUNCTIONS
//
//===========================================


//--------------------------------------------
// Initialize Motor
//--------------------------------------------
void initializeMotor() {
  //set motor pin modes
  pinMode(pwmPinLeft, OUTPUT);
  pinMode(dirPinLeft1, OUTPUT);
  pinMode(dirPinLeft2, OUTPUT);

  // set PWM frequency
  setPwmFrequency(pwmPinLeft,1);

  pinMode(pwmPinRight, OUTPUT);
  pinMode(dirPinRight1, OUTPUT);
  pinMode(dirPinRight2, OUTPUT);
  
  setPwmFrequency(pwmPinRight,1);

  // Initialize motor 
  analogWrite(pwmPinLeft, 255);     // set to not spinning voltage
  digitalWrite(dirPinLeft1, HIGH);  // set direction
  digitalWrite(dirPinLeft2, LOW);  // set direction

  analogWrite(pwmPinRight, 255);     // set to not spinning voltage
  digitalWrite(dirPinRight1, LOW);  // set direction
  digitalWrite(dirPinRight2, HIGH);  // set direction
}




//--------------------------------------------
// Pwm Frequency
// From https://playground.arduino.cc/Code/PwmFrequency
//--------------------------------------------
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}



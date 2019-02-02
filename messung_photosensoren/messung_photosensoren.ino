int thresholds[3] = { 49, 89, 180 };

//Motor Pins
int pwmPinLeft = 3; // PWM output pin for motor 1
int dirPinLeft1 = 2; // direction output pin for motor 1
int dirPinLeft2 = 4; // direction output pin for motor 1
int pwmPinRight = 5;
int dirPinRight1 = 8;
int dirPinRight2 = 9;

int rightPin = A2;
int leftPin = A1;

int leftPinLED = 13;
int rightPinLED = 12;

int indexLeft = -1;
int indexRight = -1;

int oldIndexLeft = -1;
int oldIndexRight = -1;

int waitCountRight = 0;
int waitCountLeft = 0;

bool rightDirection;
bool leftChanged = false;
bool rightChanged = false;
bool isTurning = false;
bool drivingForward = true;

int currentSpeed = 200;

unsigned long lastUpdateTime = 0;

int modulo(int dividend, int divisor) {
  return (dividend % divisor + divisor) % divisor;
}

bool setRightDirection(int newIndex, int oldIndex) {
  rightDirection = (modulo((newIndex - oldIndex), 3) == 2);  
}

void turnLeft() {
  if (drivingForward) {
    analogWrite(pwmPinLeft, 0);
    analogWrite(pwmPinRight, currentSpeed); 
  } else {
    analogWrite(pwmPinLeft, currentSpeed);
    analogWrite(pwmPinRight, 0);
  }
  digitalWrite(leftPinLED, HIGH);
  digitalWrite(rightPinLED, LOW);
}
void turnRight() {
  if (drivingForward) {
    analogWrite(pwmPinLeft, currentSpeed);
    analogWrite(pwmPinRight, 0); 
  } else {
    analogWrite(pwmPinLeft, 0);
    analogWrite(pwmPinRight, currentSpeed);
  }
  digitalWrite(leftPinLED, LOW);
  digitalWrite(rightPinLED, HIGH);
}

void drive() {
  if (drivingForward) {
    driveForward();
  } else {
    driveBackward();
  }
}

void driveForward() {
  analogWrite(pwmPinLeft, currentSpeed);     // set to not spinning voltage
  digitalWrite(dirPinLeft1, HIGH);  // set direction
  digitalWrite(dirPinLeft2, LOW);  // set direction

  analogWrite(pwmPinRight, currentSpeed);     // set to not spinning voltage
  digitalWrite(dirPinRight1, LOW);  // set direction
  digitalWrite(dirPinRight2, HIGH);
}


void driveBackward() {
  analogWrite(pwmPinLeft, currentSpeed);     // set to not spinning voltage
  digitalWrite(dirPinLeft1, LOW);  // set direction
  digitalWrite(dirPinLeft2, HIGH);  // set direction

  analogWrite(pwmPinRight, currentSpeed);     // set to not spinning voltage
  digitalWrite(dirPinRight1, HIGH);  // set direction
  digitalWrite(dirPinRight2, LOW);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(leftPinLED, OUTPUT);
  pinMode(rightPinLED, OUTPUT);
  initializeMotor();
  drive();
}

void loop() {
  // put your main code here, to run repeatedly:

   int leftSensorValue = analogRead(rightPin);
   int rightSensorValue = analogRead(leftPin);
  
  unsigned long currentTime = millis();
  if (currentTime - lastUpdateTime > 100) // executes every 100ms 
  {   
     //println();
    for (int i = 0; i < 3; i++) {
      if (rightSensorValue <= thresholds[i]) {
         if (indexRight != i && waitCountRight >= 3) {
          oldIndexRight = indexRight;
          indexRight = i;
          waitCountRight = 0;
          setRightDirection(indexRight, oldIndexRight);
          rightChanged = true;
        } else if (indexRight != i) {
          waitCountRight++;
        }
        break;
      }
    }
    
    for (int i = 0; i < 3; i++) {
      if (leftSensorValue <= thresholds[i]) {
        if (indexLeft != i && waitCountLeft >= 3) {
          oldIndexLeft = indexLeft;
          indexLeft = i;
          waitCountLeft = 0;
          setRightDirection(indexLeft, oldIndexLeft);
          leftChanged = true;
        } else if (indexLeft != i) {
          waitCountLeft++;
        }
        break;
      }
    }

    if (indexLeft == indexRight && isTurning) {
      isTurning = false;
      //fahren
      drive();
    }
   
    
    if (leftChanged || rightChanged) {
      Serial.println("=========================");
//      Serial.print(rightDirection);
//      Serial.print( " ");
//      Serial.print(oldIndexRight);
//      Serial.print( " ");
//      Serial.print(indexRight);
//      Serial.print( " ");
//      Serial.print(rightSensorValue);
//      Serial.print( " ");
//      Serial.println(modulo(indexRight-oldIndexRight, 3));
//      Serial.print( " ");
//      Serial.print(rightDirection);
//      Serial.print( " ");
//      Serial.print(oldIndexLeft);
//      Serial.print( " ");
//      Serial.print(indexLeft);
//      Serial.print( " ");
//      Serial.print(leftSensorValue);
//      Serial.print( " ");
//      Serial.println(modulo(indexLeft-oldIndexLeft, 3));
//      Serial.print( " ");
      if (!rightDirection) {
        if (leftChanged && !rightChanged) {
          // turnRight until equal
          // rechts an
          turnRight();
          isTurning = true;
        } else if (rightChanged && !leftChanged) {
          // turnLeft until equal
          // links an
          turnLeft();
          isTurning = true;
        } else {
          // vorwaerts = rückwaerts
          // blinken
          drivingForward = !drivingForward;
          drive();
        }
      } else {
        if (leftChanged && !rightChanged) {
          // turnLeft until equal
          // links an
          turnLeft();
          isTurning = true;
        } else if (rightChanged && !leftChanged) {
          // turnRight until equal
          // rechts an
          turnRight();
          isTurning = true;
        }
      }

      leftChanged = false;
      rightChanged = false;
    }
//    Serial.print("left: ");
//    Serial.print(indexLeft);
//    Serial.print(" ");
//    Serial.print(leftSensorValue);
//    Serial.print(" ");
//    Serial.print(waitCountLeft);
//    Serial.print(" right: ");
//    Serial.print(indexRight);
//    Serial.print(" ");
//    Serial.print(rightSensorValue);
//    Serial.print(" ");
//    Serial.println(waitCountRight);
    lastUpdateTime = currentTime;
    }

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

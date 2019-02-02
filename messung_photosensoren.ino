int thresholds[3] = { 49, 89, 180 };

int rightPin = A0;
int leftPin = A1;

int leftPinLED = 10;
int rightPinLED = 13;

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

unsigned long lastUpdateTime = 0;

int modulo(int dividend, int divisor) {
  return (dividend % divisor + divisor) % divisor;
}

bool setRightDirection(int newIndex, int oldIndex) {
  rightDirection = (modulo((newIndex - oldIndex), 3) == 2);  
}

void turnLeft() {
  digitalWrite(leftPinLED, HIGH);
  digitalWrite(rightPinLED, LOW);
}
void turnRight() {
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
  digitalWrite(leftPinLED, LOW);
  digitalWrite(rightPinLED, LOW);
}


void driveBackward() {
  digitalWrite(leftPinLED, HIGH);
  digitalWrite(rightPinLED, HIGH);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(leftPinLED, OUTPUT);
  pinMode(rightPinLED, OUTPUT);

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

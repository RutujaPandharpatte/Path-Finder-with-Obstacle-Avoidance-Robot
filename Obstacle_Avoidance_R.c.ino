const int usmechoPin = A0,usmtrigPin = A1,uslechoPin = A2,usltrigPin = A3,usrechoPin = A4,usrtrigPin = A5;
int motorPinA1 = 6,motorPinA2 = 5, motorPinB1 =10,motorPinB2 = 9;
const int left=1,right=2;
const int path[16]={2,1,1,2,2,2,1,2,1,1,2};
int duration,distance;
int leftd,rightd,middled,i=0,dthreshold=4;
bool isWall=false;
int leftIR, rightIR;
const int motorSpeedA= 150, motorSpeedB= 140;
bool start=true;

void readSensor(){

  leftIR= digitalRead(8);
  rightIR= digitalRead(7);
  Serial.print("\nIR Status:");
  Serial.print(leftIR);
  Serial.println(rightIR);

  analogWrite(usltrigPin, 0);
  delayMicroseconds(10);
  analogWrite(usltrigPin, 180);
  delayMicroseconds(10);
  analogWrite(usltrigPin, 0);
  duration = pulseIn(uslechoPin, 180);
  distance= duration*0.030/2;
  Serial.print("Left Distance:");
  Serial.print(distance);
  leftd=distance;

  analogWrite(usmtrigPin, 0);
  delayMicroseconds(10);
  analogWrite(usmtrigPin, 180);
  delayMicroseconds(10);
  analogWrite(usmtrigPin, 0);
  duration = pulseIn(usmechoPin, 180);
  distance= duration*0.030/2;
  Serial.print("  Middle Distance=");
  Serial.print(distance);
  middled=distance;

  analogWrite(usrtrigPin, 0);
  delayMicroseconds(10);
  analogWrite(usrtrigPin, 180);
  delayMicroseconds(10);
  analogWrite(usrtrigPin, 0);
  duration = pulseIn(usrechoPin, 180);
  distance= duration*0.030/2;
  Serial.print("  Right Distance:");
  Serial.print(distance);
  rightd=distance;

  Serial.println();
}

void rightmotion(int speedRight,int speedLeft){
  digitalWrite(motorPinA1,LOW);
  analogWrite(motorPinA2,speedRight);
  analogWrite(motorPinB1,speedLeft);
  digitalWrite(motorPinB2,LOW);
}

void leftmotion(int speedRight,int speedLeft){
  analogWrite(motorPinA1,speedRight);
  digitalWrite(motorPinA2,LOW);
  digitalWrite(motorPinB1,LOW);
  analogWrite(motorPinB2,speedLeft);
}

void forward(int speedRight,int speedLeft){
  analogWrite(motorPinA1,speedRight);
  digitalWrite(motorPinA2,LOW);
  analogWrite(motorPinB1,speedLeft);
  digitalWrite(motorPinB2,LOW);
}

void stopb(){
  digitalWrite(motorPinA1,LOW);
  digitalWrite(motorPinA2,LOW);
  digitalWrite(motorPinB1,LOW);
  digitalWrite(motorPinB2,LOW);
}

void setup(){
  Serial.begin(9600);
  delay(500);
  pinMode(usmtrigPin, OUTPUT);
  pinMode(usmechoPin, INPUT);
  pinMode(usltrigPin, OUTPUT);
  pinMode(uslechoPin, INPUT);
  pinMode(usrtrigPin, OUTPUT);
  pinMode(usrechoPin, INPUT); 
  pinMode(motorPinA1, OUTPUT);
  pinMode(motorPinA2, OUTPUT);
  pinMode(motorPinB1, OUTPUT);
  pinMode(motorPinB2, OUTPUT);
  digitalWrite(motorPinA1,LOW);
  digitalWrite(motorPinA2,LOW);
  digitalWrite(motorPinB1,LOW);
  digitalWrite(motorPinB2,LOW);
  
}

void loop(){
  readSensor();
  if(start)
    start=false;
  else if(middled<11&&(leftd<23 || (rightIR && (!leftIR)))){
    Serial.print("Taking Right");
    rightmotion(motorSpeedA,motorSpeedB);
    while(middled>11&&middled<20){
      readSensor();
    }
    // delay(330);
  }
  else if(middled<11&&(rightd<23 || (leftIR && (!rightIR)))) {
    Serial.print("Taking Left");
    leftmotion(motorSpeedA,motorSpeedB);
    while(middled>11&&middled<20){
      readSensor();
    }
    // delay(330);
  }
  else if(middled<11&&(rightd>23 || (!rightIR)) && (leftd>23 || (!leftIR)) ) {
    Serial.print("Taking Right");
    rightmotion(motorSpeedA,motorSpeedB);
    while(middled>11&&middled<20){
      readSensor();
    }
    // delay(330);
  }
  else if(leftd<10 || (rightIR && (!leftIR)) ){
    Serial.print("Left Detected!");
    forward(motorSpeedA-40,motorSpeedB);
    isWall=false;
  }
  else if(rightd<10 || (leftIR && (!rightIR)) ) {
    Serial.print("Right Detected!");
    forward(motorSpeedA, motorSpeedB-40);
    isWall=false;
  }
  else if(middled>dthreshold){
    Serial.print("Forward going");
    forward(motorSpeedA, motorSpeedB);
    isWall=false;
  }
  if(i>15){
    stopb();
    Serial.println("Bot stopped!");
  }
  
}

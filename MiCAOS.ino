#include <Servo.h>

//Define Speaker
int speakerPin = 3;

//Define Range of Movement
int left_bound = 0;
int right_bound = 150;

//Define Joystick Pins
int TopLeft = 9;
int BottomLeft = 10;
int TopRight = 11;
int BottomRight = 12;

//Define Joystick Inputs
int TL;
int TR;
int BL;
int BR;

//Define Servos
Servo tilt_servo;
Servo snap_servo;
Servo pan_servo;

//Servo Parameters
int down_position = 95;
int up_position = 120;
int tilt_neutral = 85;
int snap_neutral = 120;
int pan_neutral = 75;

//Define Buttons & Toggles
int shutterPin = 7;
int buttonPin = 0;
int shutterState;
int buttonState;
int sensitivity;
int memory;

//Define Rotary Switch Pins
int Rotary1 = A3;
int Rotary2 = A4;
int Rotary4 = A5;
int Rotary8 = 1;

//Define Switch Pin Readings
int R1;
int R2;
int R4;
int R8;

//Button Mode Parameters
int pan_dist = 45;
int tilt_dist = 30;
int count = 0;
int check = 0;
 
void setup() { 
//Attach Servos
  pan_servo.attach(5);
  tilt_servo.attach(6);
  snap_servo.attach(13);
  Serial.begin(9600);
  
//Define Joystick Inputs
  pinMode(TopLeft, INPUT);
  pinMode(TopRight, INPUT);
  pinMode(BottomLeft, INPUT);
  pinMode(BottomRight, INPUT);
  
//Define Switch Inputs
  pinMode(Rotary1, INPUT);
  pinMode(Rotary2, INPUT);
  pinMode(Rotary4, INPUT);
  pinMode(Rotary8, INPUT);
  
//Initialize Pullup Resistors
  digitalWrite(TopLeft, HIGH);
  digitalWrite(TopRight, HIGH);
  digitalWrite(BottomLeft, HIGH);
  digitalWrite(BottomRight, HIGH);
  digitalWrite(buttonPin, HIGH);
  digitalWrite(Rotary1, HIGH);
  digitalWrite(Rotary2, HIGH);
  digitalWrite(Rotary4, HIGH);
  digitalWrite(Rotary8, HIGH);
  
//Startup Melody
  pinMode(speakerPin, OUTPUT);
  wake();
  
  
//Reset Servos
  resetAll();
  
//Initial Button Check
  buttonState = digitalRead(buttonPin);
} 
 
void loop() {
//Register Rotary Readings
  R1 = digitalRead(Rotary1);
  R2 = digitalRead(Rotary2);
  R4 = digitalRead(Rotary4);
  R8 = digitalRead(Rotary8);
//  Serial.print(R1);
//  Serial.print(R2);
//  Serial.print(R4);
//  Serial.println(R8);

//Toggle Sensitivity
  if(R1 == 0) {
    sensitivity = 45;
  }
  else {
    sensitivity = 15;
  }
  
//Toggle Button Mode
  if(R2 == 0) {
    BR = digitalRead(BottomRight);
    if(BR == 0) {
      snap();
    }
    memory = digitalRead(buttonPin);
    if(memory > buttonState) {
      if(count < 9) {
        count ++;
      }
      else {
        count = 0;
      }
    }
    buttonState = memory;
    if(check != count) {
      zone(count);
    }
    check = count;
    return;
  }
  
//Button Press for Snap
  buttonState = digitalRead(buttonPin);
  if(buttonState == 0) {
    snap();
  }
  //Serial.println(buttonState);
  
  TL = digitalRead(TopLeft);
  TR = digitalRead(TopRight);
  BL = digitalRead(BottomLeft);
  BR = digitalRead(BottomRight);
//  Serial.print(TL);
//  Serial.print(TR);
//  Serial.print(BL);
//  Serial.println(BR);
  
//Tilt
  if(BL == 0) {
    moveServo(tilt_servo, 1, sensitivity);
  }
  else if(TR == 0) {
    moveServo(tilt_servo, -1, sensitivity);
  }
//Pan
  if(BR == 0) {
    moveServo(pan_servo, -1, sensitivity);
  }
  else if(TL == 0){
    moveServo(pan_servo, 1, sensitivity);
  }
}

//Define Servo Move Function
void moveServo(Servo servo, int delta, int sens) {
  int previousValue = servo.read();
  int newValue = previousValue + delta;
  if(newValue > right_bound || newValue < left_bound) {
    return;
  }
  servo.write(newValue);
  delay(sens);
}

//Define Servo Zone Function
void zoneServo(Servo servo, int zonePos) {
  while(servo.read() != zonePos) {
    if(zonePos > servo.read()) {
      moveServo(servo, 1, sensitivity);
    }
    else {
      moveServo(servo, -1, sensitivity);
    }
  }
}

//Define Servo Reset Function
void reset(Servo servo, int newPos) {
  int previousPos = servo.read();
  if (newPos > previousPos) {
    for (int i = previousPos; i < newPos; i++) {
      servo.write(i); 
      delay(15);
    }
  }
  else {
    for(int i = previousPos; i > newPos; i--) {
      servo.write(i); 
      delay(15);
    }
  }
}

//Define Reset All Servo Position
void resetAll() {
  reset(pan_servo, pan_neutral);
  reset(tilt_servo, tilt_neutral);
  reset(snap_servo, snap_neutral);
}

//Define Shutter Press Function
void snap() {
  snap_servo.write(down_position);
  delay(500);
  snap_servo.write(up_position);
  delay(800);
}

//Define Zones Switch Case
void zone(int var) {
  switch(var) {
    case 0:
      resetAll();
      break;
    case 1:
      zoneServo(pan_servo, pan_neutral - pan_dist);
      zoneServo(tilt_servo, tilt_neutral + tilt_dist);
      break;
    case 2:
      zoneServo(pan_servo, pan_neutral);
      break;
    case 3:
      zoneServo(pan_servo, pan_neutral + pan_dist);
      break;
    case 4:
      zoneServo(pan_servo, pan_neutral - pan_dist);
      zoneServo(tilt_servo, tilt_neutral);
      break;
    case 5:
      zoneServo(pan_servo, pan_neutral);
      break;
    case 6:
      zoneServo(pan_servo, pan_neutral + pan_dist);
      break;
    case 7:
      zoneServo(pan_servo, pan_neutral - pan_dist);
      zoneServo(tilt_servo, tilt_neutral - tilt_dist);
      break;
    case 8:
      zoneServo(pan_servo, pan_neutral);
      break;
    case 9:
      zoneServo(pan_servo, pan_neutral + pan_dist);
      break;
  }
}

//Startup Melody
void wake() {
  tone(speakerPin, 660, 100); delay(150);
  tone(speakerPin, 660, 100); delay(300);
  tone(speakerPin, 660, 100); delay(300);
  tone(speakerPin, 510, 100); delay(150);
  tone(speakerPin, 660, 100); delay(300);
  tone(speakerPin, 770, 100); delay(600);
  tone(speakerPin, 380, 100);
}

//Rotary Position Binary Combinations
//pos01 = 1111
//pos02 = 0111
//pos03 = 1011
//pos04 = 0011
//pos05 = 1101
//pos06 = 0101
//pos07 = 1001
//pos08 = 0001
//pos09 = 1110
//pos10 = 0110
//pos11 = 1010
//pos12 = 0010
//pos13 = 1100
//pos14 = 0100
//pos15 = 1000
//pos16 = 0000

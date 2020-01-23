#include <Servo.h>          //Servo motor library. This is standard library
#include <NewPing.h>        //Ultrasonic sensor function library. You must install this library

//our L298N control pins
const int LeftMotorForward = 7;
const int LeftMotorBackward = 6;
const int RightMotorForward = 4;
const int RightMotorBackward = 5;

//sensor pins
#define trig_pin A1 //analog input 1
#define echo_pin A2 //analog input 2

#define maximum_distance 200
boolean goesForward = false;
int distance = 100;
unsigned long delaystart = 0;

const int buzzer = 3;
const int IRpin = 9;
const int carPin = 1;
const int ledPin = 2;

NewPing sonar(trig_pin, echo_pin, maximum_distance); //sensor function
Servo servo_motor; //our servo name


void setup() {
  Serial.begin(9600);
  pinMode(RightMotorForward, OUTPUT);
  pinMode(LeftMotorForward, OUTPUT);
  pinMode(LeftMotorBackward, OUTPUT);
  pinMode(RightMotorBackward, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(IRpin, INPUT);
  pinMode(carPin,INPUT);

  servo_motor.attach(10); //our servo pin

  servo_motor.write(90);
  delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}

void loop() {

  if (digitalRead(carPin) != HIGH) {
    digitalWrite(RightMotorForward, LOW);
    digitalWrite(LeftMotorForward, LOW);
    digitalWrite(RightMotorBackward, LOW);
    digitalWrite(LeftMotorBackward, LOW);
    Serial.println("Car off");
    return;
  }
  Serial.println("Car onn");
  int distanceRight = 0;
  int distanceLeft = 0;
  boolean delayrunning = false;
  int maxdistance;

  if (digitalRead(IRpin) == 1) {
    digitalWrite(buzzer, HIGH);
    Serial.println("Edge detected, turning car by 180 degrees");
    moveStop();
    delay(300);
    moveBackward();
    delay(700);
    moveStop();
    delay(300);
    turnoneeighty();
    delay(300);
    moveForward();
    digitalWrite(buzzer, LOW);
  }
  else {
    Serial.println("No edges detected, moving car forward");
  }

  if (distance <= 40) {
    Serial.println("Obstacle ahead");
    digitalWrite(buzzer, HIGH);
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);
    digitalWrite(buzzer, LOW);

    maxdistance = distanceLeft - distanceRight;
    if (maxdistance < 0) {
      turnRight();
    } else {
      turnLeft();
    }
  }
  Serial.print(distance); Serial.print("\n");
  Serial.print(distanceRight); Serial.print("\n");
  Serial.print(distanceLeft); Serial.print("\n");
  if (distance > 40 || distance == 250) {
    Serial.println("No obstacles, moving forward");
    moveForward();
  }
  distance = readPing();
  delay(200);

}

int lookRight() {
  servo_motor.write(20);
  delay(500);
  int distance1 = readPing();
  servo_motor.write(50);
  delay(500);
  int distance2 = readPing();
  servo_motor.write(90);
  if (distance1 < 40 && distance2 > 40)
    return distance1;
  if (distance1 > 40 && distance2 < 40)
    return distance2;
  if (distance1 > 40 && distance2 > 40)
    return distance1;
  if (distance1 < 40 && distance2 < 40)
    return distance1;
}

int lookLeft() {
  servo_motor.write(160);
  delay(500);
  int distance1 = readPing();
  servo_motor.write(135);
  delay(500);
  int distance2 = readPing();
  servo_motor.write(90);
  if (distance1 < 40 && distance2 > 40)
    return distance1;
  if (distance1 > 40 && distance2 < 40)
    return distance2;
  if (distance1 > 40 && distance2 > 40)
    return distance1;
  if (distance1 < 40 && distance2 < 40)
    return distance1;
}

int readPing() {
  delay(70);
  int cm = sonar.ping_cm();
  if (cm == 0) {
    cm = 250;
  }
  return cm;
}

void moveStop() {

  digitalWrite(RightMotorForward, LOW);
  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);
  digitalWrite(LeftMotorBackward, LOW);
}

void moveForward() {

  if (!goesForward) {

    goesForward = true;

    digitalWrite(LeftMotorForward, HIGH);
    digitalWrite(RightMotorForward, HIGH);

    digitalWrite(LeftMotorBackward, LOW);
    digitalWrite(RightMotorBackward, LOW);
  }
}

void moveBackward() {

  goesForward = false;

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorForward, LOW);

}

void turnRight() {

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorBackward, HIGH);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorForward, LOW);

  delay(700);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);



}

void turnLeft() {

  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(700);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

void turnoneeighty() {
  digitalWrite(LeftMotorBackward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorForward, LOW);
  digitalWrite(RightMotorBackward, LOW);

  delay(1000);

  digitalWrite(LeftMotorForward, HIGH);
  digitalWrite(RightMotorForward, HIGH);

  digitalWrite(LeftMotorBackward, LOW);
  digitalWrite(RightMotorBackward, LOW);
}

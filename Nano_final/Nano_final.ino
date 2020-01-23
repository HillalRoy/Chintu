#include <dht.h> //Library for DHT Sensor.

//Sensor variables
float temperature, humidity, gasvalue;
boolean gasfound = false;
float flamevalue;
boolean flamefound = false;
bool isManual = false;

char CAR_ON = 'A';
char LIGHT_ON = 'B';
char CAR_OFF = 'C';
char LIGHT_OFF = 'D';
char READ_DATA = 'E';

char FORWARD = 'F';
char BACKWARD = 'G';
char LEFT = 'H';
char RIGHT = 'I';
char AUTO = 'J';
char MANUAL = 'K';
char MANUAL_ON = 'L';

#define fwardPin 2
#define bwardPin 3
#define leftPin 4
#define rightPin 5
#define modePin 7
#define buzzer 9
#define carPin 10
#define dhtapin A0  // analog input for DHT Sensor
#define gaspin A1  //analog input for smoke sensor
#define flamepin A2 //analog input for flame sensor


dht DHT;//DHT class
void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(carPin, OUTPUT);
  pinMode(fwardPin, OUTPUT);
  pinMode(bwardPin, OUTPUT);
  pinMode(leftPin, OUTPUT);
  pinMode(rightPin, OUTPUT);
  pinMode(modePin, OUTPUT);
  Serial.begin(9600);

  //  digitalWrite(bwardPin, HIGH);
  //  digitalWrite(fwardPin, HIGH);
  //  digitalWrite(bwardPin, HIGH);
  //  digitalWrite(leftPin, HIGH);
  //  digitalWrite(rightPin, HIGH);

  delay(500);
}


void printValues() {
  Serial.flush();
  Serial.print("--start--");

  Serial.print(";t:");
  Serial.print(temperature);

  Serial.print(";h:");
  Serial.print(humidity);

  Serial.print(";g:");
  Serial.print(gasvalue);

  Serial.print(";f:");
  Serial.print(flamevalue);

  Serial.println(";--end--");
  Serial.flush();
}

void switches() {

  char str = '0';
  if (Serial.available())
    str = Serial.read();



  if (str == CAR_ON) {
    digitalWrite(carPin, HIGH);
  }
  else if (str == AUTO) {
    isManual = false;
    digitalWrite(modePin, LOW);
  }
  else if (str == CAR_OFF) {
    digitalWrite(carPin, LOW);
  }

  else if (str == READ_DATA) {
    ReadEverything();
    printValues();

  }
  else if (str == FORWARD) {
    digitalWrite(bwardPin, LOW);
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, LOW);
    digitalWrite(fwardPin, HIGH);

  }
  else if (str == BACKWARD) {
    digitalWrite(fwardPin, LOW);
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, LOW);
    digitalWrite(bwardPin, HIGH);
  }
  else if (str == LEFT) {
    digitalWrite(fwardPin, LOW);
    digitalWrite(bwardPin, LOW);
    digitalWrite(rightPin, LOW);
    digitalWrite(leftPin, HIGH);
  }
  else if (str == RIGHT) {
    digitalWrite(fwardPin, LOW);
    digitalWrite(bwardPin, LOW);
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, HIGH);
  }

  else if (str == MANUAL) {
    digitalWrite(fwardPin, LOW);
    digitalWrite(bwardPin, LOW);
    digitalWrite(leftPin, LOW);
    digitalWrite(rightPin, LOW);
  }  else if (str == MANUAL_ON) {
    isManual = true;
    digitalWrite(modePin, HIGH);
  }
  if (Serial.available())
    switches();
}

void loop() {


  if (Serial.available() > 0) {
    switches();
  }


  //  if (gasfound | flamefound) digitalWrite(buzzer, HIGH);
  //  else digitalWrite(buzzer, LOW);
  //  ReadEverything();
  //  printValues();
  //  delay(2000);
}


void ReadEverything() {
  float gasthres = 700;
  float flamethres = 80;

  DHT.read11(dhtapin);
  temperature = DHT.temperature;
  humidity = DHT.humidity;

  gasvalue = analogRead(gaspin);

  if (gasvalue > gasthres) {
    gasfound = true;
  }
  else {
    gasfound = false;
  }

  flamevalue = analogRead(flamepin);

  if (flamevalue < flamethres) {
    flamefound = true;
  }
  else {
    flamefound = false;
  }
}

#include <dht.h> //Library for DHT Sensor.

//Sensor variables
float temperature, humidity;
float gasvalue;
boolean gasfound = false;
float flamevalue;
boolean flamefound = false;

const int buzzer = 3;

#define dhtapin A0  // analog input for DHT Sensor
#define gaspin A1  //analog input for smoke sensor
#define flamepin A2 //analog input for flame sensor
#define lightPin 6
#define carPin 5

dht DHT;//DHT class
void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(lightPin, OUTPUT);
  pinMode(carPin, OUTPUT);
  Serial.begin(9600);
  delay(500);
}


void printValues(){
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

void switches(){
  
  String str = "";
  while(Serial.available()){
    str += Serial.readString();
  }
  
  if(str == "CON"){
      digitalWrite(carPin, HIGH);
  }
  else if(str == "LON"){
      digitalWrite(lightPin, HIGH);
  }
    else if(str == "COFF"){
      digitalWrite(carPin, LOW);
  }
  else if(str == "LOFF"){
      digitalWrite(lightPin, LOW);
  }  
  
  
}

void loop() {
  ReadEverything();
  printValues();
  
  if(Serial.available()>0){
    switches();
  }
  if (gasfound | flamefound) digitalWrite(buzzer, HIGH);
  else digitalWrite(buzzer, LOW);
  
  delay(1500);
}


void ReadEverything() {
  float gasthres = 725;
  float flamethres = 500;

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

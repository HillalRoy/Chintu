#include <dht.h> //Library for DHT Sensor.
#include <Wire.h>

//Sensor variables
float temperature, humidity;
float gasvalue;
boolean gasfound = false;
float flamevalue;
boolean flamefound = false;
int CurrentTime = 0;
int PrevTime = 0;
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

// Dipankar don't touch
String CAR_ON = "A";
String LIGHT_ON = "B";
String CAR_OFF = "C";
String LIGHT_OFF = "D";
String READ_DATA = "E";

String FORWARD = "F";
String BACKWARD = "G";
String LEFT = "H";
String RIGHT = "I";
String AUTO = "J";
String MANUAL = "K";
String MANUAL_ON = "L";
// end

const int buzzer = 3;

#define dhtapin A0  // analog input for DHT Sensor
#define gaspin A1  //analog input for smoke sensor
#define flamepin A2 //analog input for flame sensor
#define Slave_addr 9
#define MPU_addr 0x68
bool isManual = false;
dht DHT;//DHT class
void setup() {
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  delay(500);
}

void switches() {
  if (Serial.available() <= 0) return;
  String str = "";

  while (Serial.available()) {
    String str = String(Serial.readString());
    str =  str.substring(0, str.length() - 1);

    Serial.print(str);
    Serial.println("end");

    if (str.equals(CAR_ON)) {
      SendToUno("CON");
      Serial.println("Car on");

    }
    else if (str.equals(AUTO)) {
      isManual = false;

    }
    else if (str.equals(CAR_OFF)) {
      SendToUno("COFF");
    }

    else if (str.equals(FORWARD)) {


    }
    else if (str.equals(BACKWARD)) {

    }
    else if (str.equals(LEFT)) {

    }
    else if (str.equals(RIGHT)) {

    }

    else if (str.equals(MANUAL)) {

    }  else if (str.equals(MANUAL_ON)) {
      isManual = true;

    }
  }

}

void loop() {
  CurrentTime = millis();
  getMPUval();

  switches();

  if ((CurrentTime - PrevTime) > 2000) {
    ReadEverything();
    PrevTime = millis();
  }
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

  if (gasfound | flamefound)
    digitalWrite(buzzer, HIGH);
  else
    digitalWrite(buzzer, LOW);
}

void getMPUval() {
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  Serial.flush();
  Serial.print("--start--");

  Serial.print(";ax:");
  Serial.print(AcX);

  Serial.print(";ay:");
  Serial.print(AcY);

  Serial.print(";az:");
  Serial.print(AcZ);

  Serial.print(";gx:");
  Serial.print(GyX);

  Serial.print(";gy:");
  Serial.print(GyY);

  Serial.print(";gz:");
  Serial.print(GyZ);

  Serial.println(";--end--");
  Serial.flush();
}

void SendToUno(String str) {
  Wire.beginTransmission(Slave_addr);
  Wire.write(str.c_str());
  Wire.endTransmission();
}

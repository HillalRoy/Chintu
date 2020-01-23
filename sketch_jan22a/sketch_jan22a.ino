void setup() {
  for(int i=2;i<=5;i++){
    pinMode(i,OUTPUT);
  }
  Serial.begin(9600);

}

void loop() {
  char a;
  if(Serial.available()){
  a = Serial.read();
  if(a=='w')
  Serial.println(a);
  Serial.println("Command Received");
  }
}

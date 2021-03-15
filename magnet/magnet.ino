int sensorPin = 26;
int magnetSensor; 

void setup(){
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);
}

void loop() {
  magnetSensor = digitalRead(sensorPin);  // find out switch state by reading input pin
 Serial.println(magnetSensor);
  if(magnetSensor == HIGH){
    Serial.println("Loin");
  }
  else{
    Serial.println("Prêt");
  }
  delay(200);
}

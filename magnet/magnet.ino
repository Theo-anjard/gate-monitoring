const int sensor = 26;
  
int state; // 0 close - 1 open wwitch

void setup()
{
  Serial.begin(9600);
  pinMode(sensor, INPUT_PULLUP);
}

void loop()
{
  state = digitalRead(sensor);
  
  if (state == HIGH){
     Serial.println("Loin");
  }
  else{
     Serial.println("Près");
  }
  delay(200);
}

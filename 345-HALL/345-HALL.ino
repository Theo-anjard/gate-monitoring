#define HALL_SENSOR 2
#define LED 4//the Grove - LED is connected to D4 of Arduino
const int magnetPin = 26;

//#define magnetPin 26 // int magnetPin = 26;
// #define magnetSensor // int magnetSensor; 

void setup()
{
    Serial.begin(9600);
    pinsInit();
}
void pinsInit()
{
    pinMode(HALL_SENSOR, INPUT);
    pinMode(LED,OUTPUT);
    pinMode(magnetPin, INPUT_PULLUP);
  digitalWrite(magnetPin, HIGH);
}

void loop() 
{
    if(isNearMagnetHALL())//if the hall sensor is near the magnet?
    {
            Serial.println("Près");
        turnOnLED();
    }
    else
    {
          Serial.println("loin");

        turnOffLED();
    }

    if(isNearMagnet345())//if the hall sensor is near the magnet?
    {
            Serial.println("Près345");
    }
    else
    {
          Serial.println("Loin345");
    }
delay(200);
}



boolean isNearMagnet345()
{
    int magnetSensor = digitalRead(magnetPin);
    if(magnetSensor == LOW)//if the sensor value is LOW?
    {
        return true;//yes,return ture
    }
    else
    {
        return false;//no,return false
    }
}

boolean isNearMagnetHALL()
{
    int sensorValueHALL = digitalRead(HALL_SENSOR);
    if(sensorValueHALL == LOW)//if the sensor value is LOW?
    {
        return true;//yes,return ture
    }
    else
    {
        return false;//no,return false
    }
}
void turnOnLED()
{
    digitalWrite(LED,HIGH);
    
}
void turnOffLED()
{
    digitalWrite(LED,LOW);
}

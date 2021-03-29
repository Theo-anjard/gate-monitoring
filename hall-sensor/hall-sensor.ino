#define HALL_SENSOR 2
#define LED 4//the Grove - LED is connected to D4 of Arduino
 
void setup()
{
    Serial.begin(9600);
    pinsInit();
}
 
void loop() 
{
    if(isNearMagnet())//if the hall sensor is near the magnet?
    {
            Serial.println("Près");
        turnOnLED();
    }
    else
    {
          Serial.println("loin");

        turnOffLED();
    }
}
void pinsInit()
{
    pinMode(HALL_SENSOR, INPUT);
    pinMode(LED,OUTPUT);
}
 
/*If the hall sensor is near the magnet whose south pole is facing up, */
/*it will return ture, otherwise it will return false.              */
boolean isNearMagnet()
{
    int sensorValue = digitalRead(HALL_SENSOR);
    if(sensorValue == LOW)//if the sensor value is LOW?
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

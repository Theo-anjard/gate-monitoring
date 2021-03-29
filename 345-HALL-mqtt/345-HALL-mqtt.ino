#define HALL_SENSOR 2
#define LED 4//the Grove - LED is connected to D4 of Arduino
const int magnetPin = 26;

#include <WiFi.h>
#include <WiFiMulti.h> 
#include <PubSubClient.h> //Librairie pour la gestion Mqtt 

//#define magnetPin 26 // int magnetPin = 26;
// #define magnetSensor // int magnetSensor; 

//WIFI
const char* ssid = "lpido";
const char* password = "lpido2020";
//MQTT
const char* mqtt_server = "10.202.0.63";//Adresse IP du Broker Mqtt
const int mqttPort = 1883; //port utilisé par le Broker 
long tps=0;

WiFiMulti WiFiMulti;
WiFiClient espClient;
PubSubClient client(espClient);





void setup()
{
    Serial.begin(9600);
    pinsInit();
    setup_wifi();
    setup_mqtt();
    client.publish("esp", "Hello from ESP8266");
}


void loop() 
{
  reconnect();
  delay(1000);
  client.loop();
    if(isNearMagnetHALL())//if the hall sensor is near the magnet?
    {
            Serial.println("Près");
            turnOnLED();
            mqtt_publish("esp/etage2/porte3/HALL",0);
    }
    else
    {
          Serial.println("loin");
          mqtt_publish("esp/etage2/porte3/HALL",1);

        turnOffLED();
    }

    if(isNearMagnet345())//if the hall sensor is near the magnet?
    {
            Serial.println("Près345");
            mqtt_publish("esp/etage2/porte3/345",0);
    }
    else
    {
          Serial.println("Loin345");
          mqtt_publish("esp/etage2/porte3/345",1);

    }
delay(200);
}

void pinsInit() //Identification des pins
{
    pinMode(HALL_SENSOR, INPUT);
    pinMode(LED,OUTPUT);
    pinMode(magnetPin, INPUT_PULLUP);
  digitalWrite(magnetPin, HIGH);
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

// ************************************************************************************************
void setup_wifi(){
  //connexion au wifi
  WiFiMulti.addAP(ssid, password);
  while ( WiFiMulti.run() != WL_CONNECTED ) {
    delay ( 1000 );
    Serial.print ( "." );
  }
  Serial.println("");
  Serial.println("WiFi connecté");
  Serial.print("MAC : ");
  Serial.println(WiFi.macAddress());
  Serial.print("Adresse IP : ");
  Serial.println(WiFi.localIP());
}

void setup_mqtt(){
  client.setServer(mqtt_server, mqttPort);
  client.setCallback(callback);//Déclaration de la fonction de souscription
  reconnect();
}

//Callback doit être présent pour souscrire a un topic et de prévoir une action 

void callback(char* topic, byte *payload, unsigned int length) {
   Serial.println("-------Nouveau message du broker mqtt-----");
   Serial.print("Canal:");
   Serial.println(topic);
   Serial.print("donnee:");
   Serial.write(payload, length);
   Serial.println();
  
 }

void reconnect(){
  while (!client.connected()) {
    Serial.println("Connection au serveur MQTT ...");
    if (client.connect("ESP32Client")) {
      Serial.println("MQTT connecté");
    }
    else {
      Serial.print("echec, code erreur= ");
      Serial.println(client.state());
      Serial.println("nouvel essai dans 2s");
    delay(2000);
    }
  }
  client.subscribe("esp/test/portes");//souscription au topic led pour commander une led
}

//Fonction pour publier un float sur un topic 

void mqtt_publish(String topic, float t){
  char top[topic.length()+1];
  topic.toCharArray(top,topic.length()+1);
  char t_char[50];
  String t_str = String(t);
  t_str.toCharArray(t_char, t_str.length() + 1);
  client.publish(top,t_char);
}

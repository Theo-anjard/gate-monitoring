#include <WiFi.h>
#include <WiFiMulti.h> 
#include <PubSubClient.h> //Librairie pour la gestion Mqtt 

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

void setup() {

  Serial.begin(115200);
  setup_wifi();
  setup_mqtt();
  client.publish("esp/test", "Hello from ESP8266");
}

void loop() {
  reconnect();
  delay(1000);
  client.loop(); 
  //On utilise pas un delay pour ne pas bloquer la réception de messages 
  //if (millis()-tps>10000){
    // tps=millis();
     int temp = random(2);
    // int a=1;
     mqtt_publish("esp/test/porte",temp);
      mqtt_publish("partie1/oui",temp);
     Serial.print("qqchose : ");
     Serial.println(temp); 
   //}
}

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

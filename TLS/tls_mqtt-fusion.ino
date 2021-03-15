#include <WiFi.h>
#include <WiFiMulti.h> 
#include <PubSubClient.h> //Librairie pour la gestion Mqtt 
#include <WiFiClientSecure.h>
//WIFI
const char* ssid = "lpido";
const char* password = "lpido2020";
//MQTT
const char* mqtt_server = "10.202.0.63";//Adresse IP du Broker Mqtt
const int mqttPort = 8883; //port utilisé par le Broker 
long tps=0;

WiFiMulti WiFiMulti;
//WiFiClient espClient;
WiFiClientSecure espClient;
PubSubClient client(espClient);


const char* local_root_ca =
    "-----BEGIN CERTIFICATE-----\n" \
    "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\n" \
    "Z4xBpnlfb8gBEgKCor5C1KreFEBujt0SKg==\n" \
    "-----END CERTIFICATE-----";


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

void setup() {

  Serial.begin(115200);
  espClient.setCACert(local_root_ca);
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

---
title: Monitoring des portes extérieure de l'IUT
---
# <center> Monitoring des portes extérieure de l'IUT </center>

<i class="fa fa-edit fa-fw"></i> Rédigé par : Théo Anjard 
<i class="fa fa-eye fa-fw"></i> Tuteur: Sébastien Druon
<div style="text-align:center"><img src="https://i.imgur.com/JUsfL98.png" /></div>

## Somaire

[TOC]


L'IUT est un grand batiment environ 5000m², et il dispose de nombreuses accès vers l'extérieur. Beaucoup de personnes les empreintes chaque jours, il faut donc vérifier quotidiennement à la fin de la journée que les portes soient bien fermées. Or la superficie est telle qu'il faut en moyenne une heure trente pour boucler le batiment.

### <span style="color:blue">  Problèmatique </span>
**Comment peut-on réduire le temps de fermeture de l'iut ?**

Plusieurs possibilités peuvent être envisagées : 
> 1 : 
- Dans un premier temps nous pouvons regarder au cours de la journée si certaines portes ont été ouvertes, si ce n'est pas le cas cela signifie qu'elle est encore fermée et qu'il n'y a donc pas besoin d'aller la fermée.
> 2 : 
- Nous pouvons aussi installer sur la poignet comme vu si dessous un système (capteur rotatif) qui permet de déterter l'état de la porte (verrouiller / déverrouiller) ce qui complèterait la première possiblisité.

<center> <img src="https://i.imgur.com/iGoxfYD.jpg" style="width:30%;heigh:30%;"> </center>

> 3 : 

- La version améliorée serait d'auromatiser le vérouillage et le dévérouillage de la porte.

### Énoncé du besoin


![](https://i.imgur.com/RHtNy9K.png)

### Fonction  
| Fonction |                 Enoncé                  |                       Solutions                        | Niveau |
|:-------- |:---------------------------------------:|:------------------------------------------------------:|:------:|
| FP1      | Vérifier et contrôler l'état des portes |                        Capteur                         |   ✖    |
| FC1      |              Alimentation               |                        Secteur                         |   ✖    | 
| FC2      |                Ergonomie                |                 Petit boîtier 7X4X4cm                  |  2cm   |
| FC3      |         Visualisation de l'état         |           site internet /Graphana / InfluxDB           |   ✖    |
| FC4      |      Réduction maximale des câbles      |                  Wifi public de l'iut                  |   ✖    |
| FC5      | Capable de gérer de nombreux appareils  |                       Monitoring                       |   ✖    |
| FC6      |                Sécurité                 | Chiffrement des paquets de bout en bout/LAN couche ssl |   ✖    |
| FC7      |   Alerte (ouvertures non autorisées)    |              Notification / appel / mail               |   ✖    |
| FC8      |                 Le coût                 |            réduire au maximum les dépenses             |   ✖    |
| FC9      |         Consommation de donnée          |                     Protocole MQTT                     |   ✖    |
| FC10     |              Accessibilité              |                 site web /application                  |   ✖    |
| FC11     |            Authentification             | Traçabilité de celui qui ferme des portes via site web |   ✖    |
---

### Principe

Capteur (aimant) -> esp32 -> MQTT Broker -> Graphana/Influx -> serveur web (php) / Node-RED

Capteur (aimant) : contact magnétique pour la detection de l'ouverture et la fermeture d'une porte.
![](https://i.imgur.com/wUgEUmq.png)

---
> ### Connecteur magnétique

Ce sont des capteurs TOR (tout ou rien) Avec soit la dénomination NO ou NC.
Avec NO, le courant ne passe que si l'aimant est à proximité. Avec NC c'est linverse le courant passe si il n'y a pas l'aimant à proximité.
![](https://i.imgur.com/w855O1s.png)

Pour un soucis de logique et de consommation le capteur NC consommera moins d'électricité car la porte reste plus souvent fermée qu'ouverte. Il y aura donc du courant uniquement quand la porte est ouverte.

Plusieurs options s'offraient à moi en ce qui concerne le choix du capteur qui me permettrait de connaitre l'état de la porte.

___
> ### capteur fin de course

Un capteur fin de course, ce capteur est placé aux extrémités d'une translation ou translation, il dispose d'une petite languette qui nous permet de visualiser le déplacement d'une pièce lorsqu'une force est aplliqué sur celle-ci.

![](https://i.imgur.com/k5hm8l0.png)
Cependant des défauts existent, en effet capteur doit être installé dans la gâche d'une porte pour venir en buté sur le pêne de la serrure.

![](https://i.imgur.com/o4nMqnr.png)
*Source : lesecretdelapoignee.wordpress.com*

*Sur le shémas représenté ci-dessus, la gâche correspond au numéro 1 et le pêne au numéro 3.*

Afin d'adapter ce système aux portes il nous faut faire un trou dans la porte afin d'y loger le capteur, ce qui est une contrainte majeur. 

---
> ### Capteur infrarouge 

Ce capteur permet de calculer une distance par le biais de signaux. 
Sur la représentation ci-dessous le signal trig est envoyé par le capteur, il va ensuite ricocher sur la surface de gauche. Pour ensuite être réceptionner par la partie echo du capteur. Le temps entre l'émission et la réception nous permet de connaitre la distance entre ces deux objets.

Dans notre cas le but est tant d'installer le capteur derrière le verrou afin de connaitre sa position. 

Si la porte est déverouillé et que le capteur le sait.
Il faut un tour complet pour que la porte soit vérouiller et inversement. Sur le shémas suivant la position par défaut est représentée.
![](https://i.imgur.com/cRSrHm0.png)

Lorsque la porte va se fermer le verrou ne sera pas en face du capteur il ne distinguera pas la même distance donc l'état a changé.

![](https://i.imgur.com/YftnFh7.png)


`
![](https://i.imgur.com/NO7tYPa.png)
`
> ### Capteur gyroscopique 

Un capteur gyroscopique est un capteur qui permet de calculer un angle. x, y , z seront les trois réels de l'espace vectoriel récupérés grâce à la mesure du capteur.

![](https://i.imgur.com/eHoOb0o.png)
Dans notre cas, Z ne variera pas. Nous serons donc uniquement en deux dimensions.

J'ai selectionné 3 capteurs qui sont susceptibles d'être utilisable. 

| Fontionnalité          |                                                     ADXL345                                                     |                                            MPU6050                                            |                                            MPU9250                                            |
|:---------------------- |:---------------------------------------------------------------------------------------------------------------:|:---------------------------------------------------------------------------------------------:|:---------------------------------------------------------------------------------------------:|
| Photos                 | <div style="text-align:center; width: 100px; height: 100px"><img src="https://i.imgur.com/4XZUbfP.png" /></div> | <div style="width: 100px; height: 100px;"><img src="https://i.imgur.com/kvuEBRf.png" /></div> | <div style="width: 100px; height: 100px;"><img src="https://i.imgur.com/xoCWb8f.png" /></div> |
| Gyroscope              |                                                        3                                                        |                                               3                                               |                                               3                                               |
| Tension d'alimentation |                                                   2.0V ~ 3.6V                                                   |                                        2.375V ~ 3.46V                                         |                                          2.4V ~ 3.6V                                          |
| Fonctionnement         |                                                  -40°C ~ 85°C                                                   |                                         -40°C ~ 85°C                                          |                                         -40°C ~ 85°C                                          |
| Visualisation Axe      | <div style="text-align:center; width: 100px; height: 100px"><img src="https://i.imgur.com/dZbmnsg.png" /></div> | <div style="width: 100px; height: 100px;"><img src="https://i.imgur.com/0aXWjf4.png" /></div> | <div style="width: 100px; height: 100px;"><img src="https://i.imgur.com/6UKC6Am.png" /></div> |
| Datasheet              |                 https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL345.pdf                 |         https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf         |        https://invensense.tdk.com/wp-content/uploads/2015/02/PS-MPU-9250A-01-v1.1.pdf         |
| Bus                    |                                                   SPI et I²C                                                    |                                              I²C                                              |                                          SPI et I²C                                           |
| Prix (AdaFruit)        |                                                     $17.50                                                      |                                             $6.95                                             |                                            $14.95                                             |
| Prix (Dig-Key)         |                                                     €12,28                                                      |                                             €6,22                                             |                                               Ø                                               |
| Prix (SemaGeek)        |                                                     7,14 €                                                      |                                               Ø                                               |                                               Ø                                               | 



<div style="width: 640px; height: 480px; margin: 10px; position: relative;"><iframe allowfullscreen frameborder="0" style="width:640px; height:480px" src="https://lucid.app/documents/embeddedchart/d7974af9-c0ec-463b-bd0e-a98a04dd047a" id="UUN9zf1-jvj."></iframe></div>


![](https://i.imgur.com/hsripsl.png)

### Switch Magnetique 
Résistance de la boite ESP.
![](https://i.imgur.com/NPrBh8C.png)

## TLS (Transport Layer Security)

Notre communication entre l'ESP32 et le broker MQTT est établi.
Nous pouvons via Wireshark voir ci-dessous une trame qui reprend le topic ainsi que le message envoyé.

Cependant Si nous nous avons la capacité de voir ce message, des personnes mal-intentionnées le peuvent aussi.
Pour palier ce problème nous pouvons crypter la trame envoyée.
Ainsi les données seront toujours bien transmises au broker mais de manière sécurisé.
## InfluxDB

```bash=
You have entered:
  Username:          Theo
  Organization:      IUT
  Bucket:            IDO
  Retention Period:  infinite
Confirm? (y/n): y

Config default has been stored in /home/theo/.influxdbv2/configs.
User    Organization    Bucket
Th�eo   IUT             IDO
```
mettre en place influxdb
voir grafana
avoir fait un recap


## Bibliographie
Semageek : contact magnétique
https://boutique.semageek.com/fr/911-switch-contact-magnetique-capteur-de-d-ouverture-de-porte.html
Radio Spare : contact magnétique
https://fr.rs-online.com/web/p/detecteurs-d-ouverture-de-portes-et-fenetres/1220719
Data sheet stwich magnetic : https://media.digikey.com/pdf/Data%20Sheets/Adafruit%20PDFs/375_Web.pdf



Capteur + Montage
https://shop.mchobby.be/fr/proximite-distance/911-contact-de-porte-magnetique-standard-3232100009110.html
https://wiki.mchobby.be/index.php?title=Contact_Magn%C3%A9tique

https://projetsdiy.fr/mosquitto-broker-mqtt-raspberry-pi/

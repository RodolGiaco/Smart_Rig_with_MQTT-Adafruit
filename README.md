# Smart_Rig
Remote control to turn on and off an ETH mining equipment.

Este proyecto consiste en controlar un Rig de mineria de ETH mediante la aplicacion MQTT-DASH que conecta al broker Adafruit.io para enviar a la placa ESP8266 que seria quien se esta suscribiendo al TOPICO "Rodol_Ariel/feed/rig" y el publicador que vendria siendo la APP MQTT-DASH la cual publica el estado del pulsador mecanico creado con un servo y un fin de carrera uniendo los 2 pines de la placa madre para prender/apagar el Rig en caso de falla o que se corte la luz por un periodo largo de tiempo.
![image](https://user-images.githubusercontent.com/62679123/176792507-ab842499-f3bd-4116-8aa9-f3cfa795194a.png)



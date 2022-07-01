#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<Servo.h>

#include<Adafruit_MQTT.h>
#include<Adafruit_MQTT_Client.h>

#define WLAN_SSID       "Neutrino"
#define WLAN_PASS       "Ragnar20"
#define DEBUG(a) Serial.println(a);


// CREDENCIALES ADAFRUIT
#define AIO_SERVER         "io.adafruit.com"
#define AIO_SERVERPORT     1883
#define AIO_USERNAME       "Rodol_Ariel"
#define AIO_KEY            "aio_rSFE70T3F8nC7a********"

// VARIABLES MODO SLEEP
uint32_t TIEMPO_DeepSleep = 10e6;  // Tiempo en modo deep-sleep en microsegundos
uint8_t TIEMPO_Referencia;            // Tiempo de referencia para encender 1 segundo el LED

Servo Servo1;
WiFiClient client;
Adafruit_MQTT_Client mqtt (&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Publish estadoPulsador = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/estadoPulsador", MQTT_QOS_1);
Adafruit_MQTT_Subscribe rig = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/rig", MQTT_QOS_1);


//variables auxiliares
int guardadatempAmb = -1;


void setup()
{
  Serial.begin(115200);
  //pinMode(2, OUTPUT);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  mqtt.subscribe(&rig);
  Servo1.attach(4);
  Servo1.write(0);
  
  TIEMPO_Referencia = millis();

}
void loop()
{
  leerDatosRecibidos();

}
bool MQTT_connect() {
  int8_t ret;
  //Stop if already connected.
  if (mqtt.connected()) {
    return true;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 10 seconds...");
    mqtt.disconnect();
    delay(500); // wait delayConexion seconds
    retries--;
     if (retries == 0) {
      //CONECTADO = false;
      delay(delayInicio);
      return false;
      }
  }
  Serial.println("MQTT Connected!");
  return true;
}

void publicarDatosInternet()
{
  if (MQTT_connect())
  {
      if (!estadoPulsador.publish("1"))
      {
        Serial.println(F("Error"));
      } else {
        Serial.println(F("OK!"));
      }
      delay(500);
      
      if (!estadoPulsador.publish("0"))
      {
        Serial.println(F("Error"));
      } else {
        Serial.println(F("OK!"));
      }
    }
}
void leerDatosRecibidos()
{
    if (MQTT_connect())
    {
      Adafruit_MQTT_Subscribe *subscription;
      while ((subscription = mqtt.readSubscription(5000)))
      {
        if (subscription == &rig)
        {
          char *valueModo = (char*)rig.lastread;
          Serial.print(F("Received: "));
          Serial.println(valueModo);
          String modo = String(valueModo);
          Serial.println(modo);

          if (modo.equals("1")) {
            Servo1.write(60); // angulo de servo para precionar pulsador
            delay(500);
            Servo1.write(0);
            delay(500);
            publicarDatosInternet();
          }
        }
      }
    }
  }

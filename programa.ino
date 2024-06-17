// Codigo ABelardo J Leon Bali (codigo para ESP8266) estacion Metereoligica

//Librerias a importar
#include "ESP8266WiFi.h"
#include <aREST.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>

//Definiciones
#define DHTPIN D2
#define DHTTYPE DHT22
#define MQ A0

//Init
DHT dht (DHTPIN, DHTTYPE);

//aREST

aREST rest = aREST();

//WiFi
const char* ssid = "Red_Privada";
const char* password = "14959005";

//Port
#define LISTEN_PORT 80

//Create Instance of server
WiFiServer server(LISTEN_PORT);

//Variables API

float mq, humidity, temperature;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();

  //Init Variables API
  rest.variable("temperatura", &temperature);
  rest.variable("humedad", &humidity);
  rest.variable("contaminacion", &mq);

  //Name ID
  rest.set_id("1");
  rest.set_name("SensorCasa");

  //Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  //Start Server
  server.begin();
  Serial.println("Server Started!");

  //IP
  Serial.println(WiFi.localIP());
}

void loop() {
  // put your main code here, to run repeatedly:
  //Wait Loop
  delay(1000);
  mq = analogRead(MQ); //MQ135
  humidity = dht.readHumidity(); //RH %0 - 100 (punto de rocio) 
  temperature = dht.readTemperature(); // 0 - 100 *C

  //Rest Calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}

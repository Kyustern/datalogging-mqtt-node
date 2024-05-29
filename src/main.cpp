#include <ESP8266WiFi.h>
#include <PubSubClient.h> //MQTT Client
#include <ArduinoJson.h>

#include <secret.h>

#include <temp.h>
#include <airmon.h>
#include <hygro.h>
#include <screen.h>
#include <Adafruit_NeoPixel.h>

#define STATUS_LED_PIN 2
#define STATUS_LED_BRIGHTNESS 20

#define AQI_NEOPIXEL_PIN D3

//Milis * seconds * minutes
int readSensorsInterval = 1000 * 60 * 5;

const char *mqtt_server = "192.168.1.95";
const char* mqtt_topic = "sensors/livingroom/temp_relhu";
const int mqttPort = 1883; // port utilisé par le Broker
long tps = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void blink_led(int dval = 100)
{
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(dval);
  digitalWrite(STATUS_LED_PIN, LOW);
}

void setup_wifi()
{
  WiFi.begin(home_ssid, home_password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    blink_led();
    delay(100);
  }
  Serial.println();

  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.println("Connection au serveur MQTT ...");
    if (client.connect("ESP8266", mosquitto_username, mosquitto_password))
    {
      Serial.println("MQTT connecté");
    }
    else
    {
      Serial.print("echec, code erreur= ");
      Serial.println(client.state());
      Serial.println("nouvel essai dans 2s");
      delay(2000);
    }
  }
}

// Callback doit être présent pour souscrire a un topic et de prévoir une action
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.println("-------Nouveau message du broker mqtt-----");
  Serial.print("Canal:");
  Serial.println(topic);
  Serial.print("donnee:");
  Serial.write(payload, length);
  Serial.println();

  if (topic == "esp/test/led")
  {
    if ((char)payload[0] == '1')
    {
      Serial.println("LED ON");
      digitalWrite(2, HIGH);
    }
    else
    {
      Serial.println("LED OFF");
      digitalWrite(2, LOW);
    }
  }
}

void setup_mqtt()
{
  client.setServer(mqtt_server, mqttPort);
  // client.setCallback(callback); // Déclaration de la fonction de souscription
  reconnect();
}

// Fonction pour publier un float sur un topic
void mqtt_publish_float(String topic, float t)
{
  char top[topic.length() + 1];
  topic.toCharArray(top, topic.length() + 1);
  char t_char[64];
  String t_str = String(t);
  t_str.toCharArray(t_char, t_str.length() + 1);
  client.publish(top, t_char);
}

void mqtt_publish_string(String topic, String payload)
{
  char top[topic.length() + 1];
  char payload_cstr[1024];
  topic.toCharArray(top, topic.length() + 1);
  payload.toCharArray(payload_cstr, payload.length() + 1);
  client.publish(top, payload_cstr);
}

//Global variables used for sensor readings
DynamicJsonDocument doc(1024);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, AQI_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

AHT_Reading thReading;
ENS_Reading ensReading;


uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t blue = strip.Color(0, 0, 255);


void readSensors () {

  tps = millis();

  getAhtReading(&thReading);
  setTemperatureCompensation(&thReading);
  getAirmonReading(&ensReading);

  if (ensReading.aqi != 0) {
    switch (ensReading.aqi) {
    case 1:
      strip.setPixelColor(0, green);
      break;
    case 2:
      strip.setPixelColor(0, green);
      break;
    case 3:
      strip.setPixelColor(0, strip.Color(255, 55, 0));
      break;
    case 4:
      strip.setPixelColor(0, red);
      break;
    default:
      break;
    }
    strip.show();
  }
  


  doc["AHT21"]["temperature"] = thReading.temperature;
  doc["AHT21"]["humidity"] = thReading.humidity;

  if (ensReading.tvoc > 0 || ensReading.eco2 > 0 || ensReading.aqi > 0)
  {
    digitalWrite(STATUS_LED_PIN, HIGH);
    doc["ENS160"]["aqi"] = ensReading.aqi;
    doc["ENS160"]["tvoc"] = ensReading.tvoc;
    doc["ENS160"]["eco2"] = ensReading.eco2;
  }

  String JSON_Out;
  serializeJson(doc, JSON_Out);
  mqtt_publish_string(String(mqtt_topic), JSON_Out);
}

void setup()
{
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(AQI_NEOPIXEL_PIN, OUTPUT);

  Serial.begin(9600);

  //INIT COMS
  setup_wifi();
  setup_mqtt();

  //INIT SENSORS
  initAHT();
  initAirmon();
  // initScreen();

  Serial.println("---------- SERVICES OK ----------");

  doc["meta"]["location"] = "living_room";
  doc["meta"]["node_description"] = "Reads temperature and humidity from the living room";

  ENS_Reading testReading;

  testReading.tvoc = 420;
  testReading.eco2 = 69;
  testReading.aqi = 1;

  delay(1000);

  getAirmonReading(&testReading);

  // drawValues(&testReading);

  digitalWrite(STATUS_LED_PIN, STATUS_LED_BRIGHTNESS);
  delay(1000);
  digitalWrite(STATUS_LED_PIN, LOW);
  delay(1000);
  digitalWrite(STATUS_LED_PIN, STATUS_LED_BRIGHTNESS);
  delay(1000);
  digitalWrite(STATUS_LED_PIN, LOW);

  readSensors();
  // String JSON_Out;
  // serializeJson(doc, JSON_Out);
  // mqtt_publish_string(String(cactusTopic), JSON_Out);
}
// serializeJson(doc, Serial);


void loop()
{
  //To clarify
  reconnect();
  client.loop();

  // On evite l'utilisation de delay() pour ne pas bloquer la boucle d'execution
  if (millis() - tps > readSensorsInterval) readSensors();
}
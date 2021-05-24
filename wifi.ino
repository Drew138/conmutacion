#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

float temp = 0;

// ---------------------Global vars------------------------------

int contconexion = 0;
const char *ssid = "Laura";
const char *password = "123456689";
int R = 10;
int G = 11;
int B = 12;
int TEMP = 13;
int LIGHT = 14;
int ANGLE = 15;

WiFiClient client;

void setup()
{
  Serial.begin(115200);
  Serial.println("");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && contconexion < 50)
  {
    ++contconexion;
    delay(500);
    Serial.print(".");
  }
  if (contconexion < 50)
  {
    IPAddress ip(10, 162, 140, 132);
    IPAddress gateway(10, 162, 140, 143);
    IPAddress subnet(255, 255, 255, 255);
    WiFi.config(ip, gateway, subnet);

    Serial.println("");
    Serial.println("WiFi conectado");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("");
    Serial.println("Error de conexion");
  }
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B OUTPUT);
  pinMode(ANGLE, OUTPUT);
  pinMode(TEMP, INPUT);
  pinMode(LIGHT, INPUT);
}

void sendRequest()
{

  StaticJsonBuffer<300> JSONbuffer; //Declaring static JSON buffer
  JsonObject &JSONencoder = JSONbuffer.createObject();

  JSONencoder["temp"] = analogRead(TEMP);
  JSONencoder["light"] = analogRead(LIGHT);

  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  //    Serial.println(JSONmessageBuffer);

  HTTPClient http;                                                //Declare object of class HTTPClient
  http.begin(client, "http://www.drew-graphics.site:8000/write"); //Specify request destination
  http.addHeader("Content-Type", "application/json");             //Specify content-type header

  int httpCode = http.POST(JSONmessageBuffer); //Send the request
  String payload = http.getString();           //Get the response payload

  if (httpCode == 200)
  {
    // Allocate JsonBuffer
    // Use arduinojson.org/assistant to compute the capacity.
    const size_t capacity = JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(2) + 60;
    DynamicJsonBuffer jsonBuffer(capacity);

    // Parse JSON object
    JsonObject &root = jsonBuffer.parseObject(payload);
    if (!root.success())
    {
      Serial.println(F("Parsing failed!"));
      return;
    }

    // Decode JSON/Extract values
    Serial.println(F("Response:"));
    Serial.println(root["R"].as<char *>());
    Serial.println(root["G"].as<char *>());
    Serial.println(root["B"].as<char *>());
    Serial.println(root["angle"].as<char *>());
    analogWrite(R, root["R"]);
    analogWrite(G, root["G"]);
    analogWrite(B, root["B"]);
    analogWrite(ANGLE, root["angle"]);
  }
  else
  {
    Serial.println("Error in response");
  }
  // Serial.println(httpCode); //Print HTTP return code
  // Serial.println(payload);  //Print request response payload

  //    http.end();  //Close connection

  delay(3000);
}

void loop()
{
  //if (WiFi.status() == WL_CONNECTED) {
  sendRequest();
  //  } else {

  //    Serial.println("Error in WiFi connection");
  // }
}

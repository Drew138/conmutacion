#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

float temp = 0;

// ---------------------Global vars------------------------------

int contconexion = 0;
const char *ssid = "Laura";
const char *password = "123456689";

int contador = 0;

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
}

void sendRequest()
{

  StaticJsonBuffer<300> JSONbuffer; //Declaring static JSON buffer
  JsonObject &JSONencoder = JSONbuffer.createObject();

  JSONencoder["temp"] = String(contador);

  char JSONmessageBuffer[300];
  JSONencoder.prettyPrintTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
  //    Serial.println(JSONmessageBuffer);

  HTTPClient http;                                                //Declare object of class HTTPClient
  http.begin(client, "http://www.drew-graphics.site:8000/write"); //Specify request destination
  http.addHeader("Content-Type", "application/json");             //Specify content-type header

  int httpCode = http.POST(JSONmessageBuffer); //Send the request
  Serial.print("xd");
  //    String payload = http.getString();                                        //Get the response payload

  //    Serial.println(httpCode);   //Print HTTP return code
  //    Serial.println(payload);    //Print request response payload

  //    http.end();  //Close connection

  delay(3000);
}

void loop()
{
  ++contador;
  //if (WiFi.status() == WL_CONNECTED) {
  //  for (int i = 0; i < 10 ; ++i)
  sendRequest();
  //  } else {

  //    Serial.println("Error in WiFi connection");
  // }
}

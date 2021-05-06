#include <ESP8266WiFi.h>
#include <WiFiClient.h>

float temp = 0;

// ---------------------Global vars------------------------------

int contconexion = 0;
const char *ssid = "Tenda_37A548";
const char *password = "9tMasdasd";

unsigned long previousMillis = 0;

char host[48];

String strhost = "www.asdasd.site";

String strurl = "/write";

String chipid = "";

String enviardatos(String datos)
{
    String linea = "error";
    strhost.toCharArray(host, 49);
    if (!client.connect(host, 80))
    {
        Serial.println("Fallo de conexion");
        return linea;
    }

    client.print(String("Post ") + strurl + datos + "HTTP/1.1" + "\r\n" +
                 "Host: " + strhost + "\r\n" + "Accept: */*" + "*\r\n" +
                 "Content-Length: " + datos.length() + "\r\n" +
                 "Content-Type: application/json" + "\r\n" + "\r\n" + datos);
    delay(10);

    Serial.println("Enviando Datos...");
    unsigned long timeout = millis();
    while (client.available() == 0)
    {
        if (millis() - timeout > 5000)
        {
            Serial.println("Cliente fuera de tiempo!");
            client.stop();
            return linea;
        }
    }

    while (client.available())
    {
        linea = client.readStringUntil('\r');
    }
    Serial.println(linea);
    return linea;
}

void setup()
{
    Serial.brin(115200);
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
        IPAddress ip(192, 168, 1, 156);      //
        IPAddress gateway(192, 168, 1, 254); // 1?
        IPAddress subnet(255, 255, 255, 0);
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

void loop()
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= 10000)
    {
        previousMillis = currentMillis;
        int analog = analogread(17);
        temp = temp + 1.0;
        Serial.println(temp);
        enviardatos(String(temp, 2));
        delay(5000);
    }
}
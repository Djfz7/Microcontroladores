// librerias
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);
// variables del conexion
#define RXD2 16
#define TXD2 17

// información del hotspot
const char* ssid = "Djfz's Wifi";
const char* password = "elmamutcito";


// variable que recibe desde la web
const char* PARAM_INPUT_1 = "input1";

// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <title>Proyecto Final</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style type="text/css">
  html, 
  body {
  height: 100%;
  }
  
  body {
  background-image: url(https://image.freepik.com/vector-gratis/fondo-abstracto-luces-azules_1182-609.jpg);
  background-repeat: no-repeat;
  background-size: 100% 100%;
  }
  p {color:white;}
  .centrado{text-align:center;border:1px dotted #FFF; padding:8px; color: #FFFFFF;}
  </style>
  
  </head>
  <body>
  <h1 class ="centrado">Proyecto final</h1>
  <p style="font-size:40px">Ingrese el mensaje a enviar:</p>
  <form action="/get">
    <input type="text" name="input1">
    <input type="submit" value="Enviar">
  </form><br>
</body></html>)rawliteral";

// si la pagina no se encuentra sale esto
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("¡Falló WiFi!");
    return;
  }

  // aqui por medio del monitor serial te dice cual es la ip que debes usar
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/get?input1=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      String hola = '}' + inputMessage + '`';
      while(Serial2.available()){
        Serial2.println(hola);
        break;
      
      }
    }
    else {
      inputMessage = "No se envio el mensaje";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/html", "Se ha enviado satisfactoriamente el siguiente mensaje: " + inputMessage +"<br><a href=\"/\"> Return to Homepage</a>");
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  // como es asincrona no hay nada en el loop
}

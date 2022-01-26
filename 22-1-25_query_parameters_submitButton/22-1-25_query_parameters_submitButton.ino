#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <painlessMesh.h>

const char* ssid = "Wireless Controller";
const char* password = "12345678";

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

AsyncWebServer server(80);

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}
IPAddress IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

void setup()
{
  Serial.begin(115200);



  // Initialize SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }



  WiFi.softAP(ssid, password);
  delay(500);
  WiFi.softAPConfig(IP, gateway, subnet);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);





  // Index.HTML NAVIGATION BUTTIONS ************************************************

  server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/index.html", String(), false);
  });

  server.on("/selection.html", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(SPIFFS, "/selection.html", String(), false );
  });









 server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
 {

   int paramsNr = request->params();
   Serial.println(paramsNr);

   for (int i = 0; i < paramsNr; i++)
   {
     AsyncWebParameter* p = request->getParam(i);
     Serial.print("Param name: ");
     Serial.println(p->name());
     
     Serial.print("Param value: ");
     Serial.println(p->value());
     
     Serial.println("------");
   }

   request->send(200, "text/plain", "message received");
 });





  server.onNotFound(notFound);

  server.begin();
}



void loop() {}




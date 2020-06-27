#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>
#include <ETH.h>

#define ETH_ADDR        1
#define ETH_POWER_PIN   5
#define ETH_MDC_PIN     23
#define ETH_MDIO_PIN    18
#define ETH_TYPE        ETH_PHY_IP101

AsyncWebServer server(80);
static bool eth_connected = false;

const int LOCK_CONTROL = 12;

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "ever";
const char* password = "06300630";

const char* PARAM_ID = "input_ID";
const char* PARAM_PW = "input_PW";

const char* PARAM_ID_CHECK = "input_id_check";
const char* PARAM_PW_CHECK = "input_pw_check";

IPAddress local_ip(192, 168, 0, 41);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(210, 94, 0, 73);

int account = 0 ;

struct event { //구조체 정의

  char date_e[20];
  char time_e[20];
  char status_e[20];

};
String processor(const String& var)
{
  if(var == "EVENT")
    return F("이벤트가 발생했습니다. ");
    Serial.println("버튼이 눌렸습니다22222"); 
  return String();
}

struct event a[10]; // 10명의 사용자를 받겠다.

void WiFiEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println("ETH Started");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;

    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println("ETH Connected");
      break;

    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      Serial.print(", ");
      Serial.print(ETH.subnetMask());
      Serial.print(", ");
      Serial.print(ETH.gatewayIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      eth_connected = true;
      break;

    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;

    case SYSTEM_EVENT_ETH_STOP:
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;

    default:
      break;
  }
}

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Mounting FS...");

  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }

  WiFi.onEvent(WiFiEvent);
  ETH.begin(ETH_ADDR, ETH_POWER_PIN, ETH_MDC_PIN, ETH_MDIO_PIN, ETH_TYPE);
  ETH.config(local_ip, gateway, subnet, dns);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Send web page with input fields to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
 request->send(SPIFFS, "/eeeee.html");
  });

 server.on("/event_up", HTTP_GET, [](AsyncWebServerRequest * request) {

  String var1 ;
  Serial.println("추가 버튼이 눌렸습니다"); 
  
 if (digitalRead(LOCK_CONTROL)){
  
    var1 = "문이 열렸습니다. " ;
  }
  
  else { var1 = "문이 닫혔습니다" ;}

 
 request->send(200, "text/plain", var1.c_str() );
 
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {
}

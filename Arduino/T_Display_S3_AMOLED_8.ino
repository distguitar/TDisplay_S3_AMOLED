#include <WiFi.h>
#include <NetworkClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Arduino.h>
#include <Arduino_GFX_Library.h> // https://github.com/moononournation/Arduino_GFX.git

const int led = 38;

/////////////////////////////////////////////////////////////////
// WiFi credentials
/////////////////////////////////////////////////////////////////
const char *ssid = "<SSID>";
const char *password = "<PASS>";

WebServer server(80);
#define GFX_DEV_DEVICE LILYGO_T_DISPLAY_S3_AMOLED

// For Display 536 x 240
Arduino_DataBus *bus = new Arduino_ESP32QSPI(
    6 /* cs */, 47 /* sck */, 18 /* d0 */, 7 /* d1 */, 48 /* d2 */, 5 /* d3 */);
Arduino_GFX *gfx_base = new Arduino_RM67162(bus, 17 /* RST */, 3 /* rotation */);
Arduino_Canvas *gfx = new Arduino_Canvas(536, 240, gfx_base);

/////////////////////////////////////////////////////////////////
// Main Setup
/////////////////////////////////////////////////////////////////
void setup(void) {

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  delay(4000);
  displaySetup();

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  SetupHTTP();

  // Test UI
  BuildCPUUI();
  BuildRAMUI(50);
  BuildLANUI(50);
  BuildDriveUI(40,0);
  BuildDriveUI(10,1);
  BuildDriveUI(80,2);
  BuildDriveUI(60,3);
  BuildTextLabels();
  gfx->flush();
}

/////////////////////////////////////////////////////////////////
// Main Loop
/////////////////////////////////////////////////////////////////
void loop(void) {
  server.handleClient();
  delay(2);  //allow the cpu to switch to other tasks
}

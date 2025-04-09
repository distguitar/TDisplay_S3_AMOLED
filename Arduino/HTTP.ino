///////////////////////////////////////////////////////////////////////////////////////
// Setup Webserver
///////////////////////////////////////////////////////////////////////////////////////
void SetupHTTP()
{
  server.on("/", handleRoot);
  server.on("/clear", handleClear);
  server.on("/data",handleData);

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

/////////////////////////////////////////////////////////////////
// Main Data Update Method GET /data
// Query String Parameters:
// r - ram (0-100)
// l - lan (0-100)
// c - cpu (0-100)
// dc - drive c (0-100)
// dd - drive d (0-100)
// df - drive f (0-100)
// dg - drive g (0-100)
/////////////////////////////////////////////////////////////////
void handleData()
{
  int ram = server.arg("r").toInt();
  int lan = server.arg("l").toInt();
  int cpu = server.arg("c").toInt();
  int dc = server.arg("dc").toInt();
  int dd = server.arg("dd").toInt();
  int df = server.arg("df").toInt();
  int dg = server.arg("dg").toInt();

  BuildRAMUI(ram);
  BuildLANUI(lan);
  BuildDriveUI(dc,0);
  BuildDriveUI(dd,1);
  BuildDriveUI(df,2);
  BuildDriveUI(dg,3);
  UpdateCPUTick(cpu);
  BuildCPUUI();
  gfx->flush();
  server.send(200, "text/plain", "Updated");
}

/////////////////////////////////////////////////////////////////
// Clear screen GET /clear
/////////////////////////////////////////////////////////////////
void handleClear()
{
  gfx->fillScreen(BLACK);
  gfx->flush();
  server.send(200, "text/plain", "Cleared Screen");
}

/////////////////////////////////////////////////////////////////
// Default GET /
/////////////////////////////////////////////////////////////////
void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp32!");
}

/////////////////////////////////////////////////////////////////
// 404 Not Found
/////////////////////////////////////////////////////////////////
void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}
/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  Changed sketck for 2 pushbuttons by Nicu FLORICA (niq_ro): http://nicuflorica.blogspot.com/
*********/

#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

const int output1 = 2;  // D4 (internal led)
const int output2 = 14; // D5


// HTML web page
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
  <head>
    <title>ESP 2 Pushbuttons Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 30px;}
      .button {
        padding: 10px 20px;
        font-size: 24px;
        text-align: center;
        outline: none;
        color: #fff;
        background-color: #2f4468;
        border: none;
        border-radius: 5px;
        box-shadow: 0 6px #999;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
      }  
      .button:hover {background-color: #1f2e45}
      .button:active {
        background-color: #1f2e45;
        box-shadow: 0 4px #666;
        transform: translateY(2px);
      }
    </style>
  </head>
  <body>
    <h1>ESP8266/ESP32: 2 Pushbuttons WebServer</h1>
    <br><br>
    <button class="button" onmousedown="toggleCheckbox('on1');" ontouchstart="toggleCheckbox('on1');" onmouseup="toggleCheckbox('off1');" ontouchend="toggleCheckbox('off1');">LED PUSHBUTTON 1</button>
    <p>
    <button class="button" onmousedown="toggleCheckbox('on2');" ontouchstart="toggleCheckbox('on2');" onmouseup="toggleCheckbox('off2');" ontouchend="toggleCheckbox('off2');">LED PUSHBUTTON 2</button>
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/" + x, true);
     xhr.send();
   }
  </script>
  <br><br><br><br><br><br><br>
  <h3>original with 1 pushbutton: <a href="https://randomnerdtutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/"
  target=_blank>https://randomnerdtutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/</a>  by Rui Santos</h3>
  <br><br>
  <h3>feature with 2 pushbuttons: 
 <a href="https://www.arduinotehniq.com/" target=_blank>https://www.arduinotehniq.com/</a>
  by Nicu FLORICA</h3>

  </body>
</html>)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    return;
  }
  Serial.println();
  Serial.print("ESP IP Address: http://");
  Serial.println(WiFi.localIP());
  
  pinMode(output1, OUTPUT);
  digitalWrite(output1, LOW);
  pinMode(output2, OUTPUT);
  digitalWrite(output2, LOW);
  
  
  // Send web page to client
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Receive an HTTP GET request
  server.on("/on1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(output1, HIGH);
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/off1", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(output1, LOW);
    request->send(200, "text/plain", "ok");
  });

   // Receive an HTTP GET request
  server.on("/on2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(output1, HIGH);
    request->send(200, "text/plain", "ok");
  });

  // Receive an HTTP GET request
  server.on("/off2", HTTP_GET, [] (AsyncWebServerRequest *request) {
    digitalWrite(output1, LOW);
    request->send(200, "text/plain", "ok");
  });

  
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
 
}

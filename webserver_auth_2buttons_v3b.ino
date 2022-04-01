/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp32-esp8266-web-server-http-authentication/
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  changed sketch by Nicu FLORICA (niq_ro) - http://nicuflorica.blogspot.com/
  ver.1.0 - changed slider with pushbutton
  ver.2.0 - added functional state of output
  ver.3.0 - added 2nd pushbutton 
  ver.3.a - internal led for cotrol, other 2 outputs 
  ver.3.b - state for internal LED
*********/

// Import required libraries
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

// Replace with your network credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

const char* http_username = "admin";
const char* http_password = "admin";

const char* PARAM_INPUT_1 = "state";

const int output0 = 2;  // D4 - internal LED
const int output1 = 12; // D6 - https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
const int output2 = 14; // D5

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 2.6rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 10px;}
     button {
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
  <h2>ESP Web Server</h2>
  <button onclick="logoutButton()">Logout</button>
  <p>Internal led GPIO 2 (D4): <span id="state0">%STATE0%</span></p>
  <p>
  <br>
  <p>Ouput GPIO 14 (D6): <span id="state1">%STATE1%</span></p>
  <p>
  <button class="button" onmousedown="toggleCheckbox(onButton1());" ontouchstart="toggleCheckbox(onButton1());" onmouseup="toggleCheckbox(offButton1());" ontouchend="toggleCheckbox(offButton1());">PUSHBUTTON 1</button>
  <p>
  <br>
  <p>Ouput GPIO 12 (D5): <span id="state2">%STATE2%</span></p>
  <p>
  <button class="button" onmousedown="toggleCheckbox(onButton2());" ontouchstart="toggleCheckbox(onButton2());" onmouseup="toggleCheckbox(offButton2());" ontouchend="toggleCheckbox(offButton2());">PUSHBUTTON 2</button>
 <p>
 <br>  
<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/" + element, true);
  xhr.send();
}
function logoutButton() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/logout", true);
  xhr.send();
  setTimeout(function(){ window.open("/logged-out","_self"); }, 1000);
}
function onButton1() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?state=1", true);
  document.getElementById("state1").innerHTML = "HIGH";
  document.getElementById("state0").innerHTML = " ON";  
  xhr.send();
}
function offButton1() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?state=0", true);
  document.getElementById("state1").innerHTML = "LOW"; 
  document.getElementById("state0").innerHTML = "OFF"; 
  xhr.send();
}
function onButton2() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?state=3", true);
  document.getElementById("state2").innerHTML = "HIGH";
  document.getElementById("state0").innerHTML = " ON";  
  xhr.send();
}
function offButton2() {
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/update?state=2", true);
  document.getElementById("state2").innerHTML = "LOW"; 
  document.getElementById("state0").innerHTML = "OFF"; 
  xhr.send();
}
</script>
  <br><br>
  <h5>original with 1 pushbutton: <a href="https://randomnerdtutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/"
  target=_blank>https://randomnerdtutorials.com/esp32-esp8266-web-server-outputs-momentary-switch/</a>  by Rui Santos
  <br><br>
  authentification webpage: <a href="https://randomnerdtutorials.com/esp32-esp8266-web-server-http-authentication/"
  target=_blank>https://randomnerdtutorials.com/esp32-esp8266-web-server-http-authentication/</a>  by Rui Santos
  <br><br>
  feature with 2 pushbuttons: 
 <a href="https://www.arduinotehniq.com/" target=_blank>https://www.arduinotehniq.com/</a>
  by Nicu FLORICA</h5>

</body>
</html>
)rawliteral";

const char logout_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <p>Logged out or <a href="/">return to homepage</a>.</p>
  <p><strong>Note:</strong> close all web browser tabs to complete the logout process.</p>
</body>
</html>
)rawliteral";

// Replace section in your web page
String processor(const String& var){
  if (var == "STATE1"){  // after restart/refresh
    if(digitalRead(output1)){
    //  Serial.println(var);
      return "HIGH";
    }
    else {
      return "LOW";
    }
  }
    if (var == "STATE2"){  // after restart/refresh
    if(digitalRead(output2)){
     // Serial.println(var);
      return "HIGH";
    }
    else {
      return "LOW";
    }
  }
   if (var == "STATE0"){  // after restart/refresh
    if(digitalRead(output0)){
      //Serial.println(var);
      return "OFF";
    }
    else {
      return " ON";
    }
  }
  return String();
}

String outputState(){
  if(digitalRead(output1)){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(output1, OUTPUT);
  digitalWrite(output1, LOW);
  pinMode(output2, OUTPUT);
  digitalWrite(output2, LOW);
  pinMode(output0, OUTPUT);
  digitalWrite(output0, HIGH);  
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    request->send_P(200, "text/html", index_html, processor);
  });
    
  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(401);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", logout_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    if(!request->authenticate(http_username, http_password))
      return request->requestAuthentication();
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      Serial.print("inputMessage.toInt() = ");
      Serial.println(inputMessage.toInt());
      if (inputMessage.toInt() == 0) 
      {
        digitalWrite(output1, 0);
        digitalWrite(output0, 1);   // internal LED off
      }
      if (inputMessage.toInt() == 1)
      {
        digitalWrite(output1, 1);
        digitalWrite(output0, 0);  // internal LED on
      }
      if (inputMessage.toInt() == 2)
      {
        digitalWrite(output2, 0);
        digitalWrite(output0, 1);   // internal LED off
      }
      if (inputMessage.toInt() == 3)
      {
        digitalWrite(output2, 1);
        digitalWrite(output0, 0);  // internal LED on
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });
  
  // Start server
  server.begin();
}
  
void loop() {
  
} // end main loop

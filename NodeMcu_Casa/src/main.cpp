#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "TecAlumnos";
const char* password = "Tec#48rrDA1";

ESP8266WebServer server(80);

const int ledPins[3] = {5, 4, 14};
bool ledStates[3] = {false, false, false};

// Genera la página HTML con tu nuevo diseño elegante
String generarPagina() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="es">
<head>
<meta charset="UTF-8">
<title>Panel Elegante de Focos</title>
<link href="https://fonts.googleapis.com/css2?family=Montserrat:wght@400;600;700&display=swap" rel="stylesheet">
<style>
* { box-sizing: border-box; margin: 0; padding: 0; }
body { font-family: 'Montserrat', sans-serif; background: linear-gradient(135deg, #0a0a0a, #1c1c1c, #2c2c2c); color: #fff; min-height: 100vh; display: flex; flex-direction: column; align-items: center; }
header { width: 100%; padding: 40px 0; text-align: center; font-size: 36px; font-weight: 700; letter-spacing: 2px; background: rgba(255,255,255,0.05); backdrop-filter: blur(10px); box-shadow: 0 5px 20px rgba(0,0,0,0.5); margin-bottom: 50px; }
.container { display: grid; grid-template-columns: repeat(auto-fit, minmax(260px, 1fr)); gap: 40px; width: 100%; max-width: 1200px; padding: 0 20px 50px 20px; }
.card { background: rgba(255,255,255,0.05); backdrop-filter: blur(15px); border-radius: 25px; padding: 35px 25px; display: flex; flex-direction: column; align-items: center; transition: transform 0.3s ease, box-shadow 0.3s ease, background 0.5s; box-shadow: 0 8px 30px rgba(0,0,0,0.7); }
.card:hover { transform: translateY(-10px); box-shadow: 0 15px 45px rgba(0,0,0,0.9); }
.card h2 { font-size: 26px; font-weight: 600; margin-bottom: 20px; letter-spacing: 1px; }
.status { font-size: 18px; margin-bottom: 25px; font-weight: 500; color: #fff; transition: color 0.3s; }
.card.on { background: rgba(255,255,200,0.1); box-shadow: 0 8px 40px rgba(255,255,180,0.6); }
.switch { position: relative; display: inline-block; width: 80px; height: 40px; perspective: 100px; }
.switch input { opacity: 0; width: 0; height: 0; }
.slider { position: absolute; cursor: pointer; top: 0; left: 0; right: 0; bottom: 0; background: #444; border-radius: 40px; transition: 0.4s; box-shadow: inset 0 0 5px rgba(0,0,0,0.5), 0 4px 8px rgba(0,0,0,0.3); }
.slider:before { position: absolute; content: ""; height: 34px; width: 34px; left: 3px; bottom: 3px; background: linear-gradient(145deg, #fff, #ccc); border-radius: 50%; transition: 0.4s; box-shadow: 0 2px 4px rgba(0,0,0,0.5), inset 0 1px 2px rgba(255,255,255,0.5); }
input:checked + .slider { background: #4CAF50; box-shadow: inset 0 0 5px rgba(0,0,0,0.5), 0 4px 10px rgba(0,0,0,0.6); }
input:checked + .slider:before { transform: translateX(40px); background: linear-gradient(145deg, #bfff9e, #8fd98f); }
.on { color: #4CAF50; font-weight: 700; }
.off { color: #f44336; font-weight: 700; }
@media (max-width: 768px) { .container { gap: 30px; } }
</style>
</head>
<body>
<header>💡 Control Inteligente de Focos</header>
<div class="container">
)rawliteral";

  // Genera tarjetas dinámicamente según ledStates
  for (int i = 0; i < 3; i++) {
    html += "<div class='card";
    if (ledStates[i]) html += " on";
    html += "'>";
    html += "<h2>HABITACION " + String(i+1) + "</h2>";
    html += "<p class='status " + String(ledStates[i] ? "on" : "off") + "'>" + String(ledStates[i] ? "Encendido" : "Apagado") + "</p>";
    html += "<label class='switch'>";
    html += "<input type='checkbox' onclick='toggleLED(" + String(i) + ")' ";
    html += (ledStates[i] ? "checked" : "");
    html += ">";
    html += "<span class='slider'></span>";
    html += "</label>";
    html += "</div>";
  }

  html += R"rawliteral(
</div>
<script>
function toggleLED(id){
  var xhr = new XMLHttpRequest();
  xhr.open("GET","/toggle?led="+id,true);
  xhr.send();
  setTimeout(()=>{location.reload();},200);
}
</script>
</body>
</html>
)rawliteral";

  return html;
}

void handleRoot() {
  server.send(200, "text/html", generarPagina());
}

void handleToggle() {
  if (server.hasArg("led")) {
    int index = server.arg("led").toInt();
    if (index >= 0 && index < 3) {
      ledStates[index] = !ledStates[index];
      digitalWrite(ledPins[index], ledStates[index] ? HIGH : LOW);
    }
  }
  handleRoot(); // Refresca la página después de cambiar el estado
}

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 3; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado a WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/toggle", handleToggle);
  server.begin();
  Serial.println("Servidor web iniciado");
}

void loop() {
  server.handleClient();
}
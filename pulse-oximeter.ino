//MAX30100 ESP32 WebServer
#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

float BPM, SpO2;

/Put your SSID & Password/
const char* ssid = "";  // Enter SSID here
const char* password = "";  //Enter Password here

PulseOximeter pox;
uint32_t tsLastReport = 0;

WebServer server(80);

void onBeatDetected()
{
  Serial.println("Beat Detected!");
}

void setup() {
  Serial.begin(115200);
  pinMode(19, OUTPUT);
  delay(100);
  Serial.println("Connecting to ");
  Serial.println(ssid);


  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());
  
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
  
  Serial.print("Initializing pulse oximeter..");

  if (!pox.begin()) {
    Serial.println("FAILED");
    for (;;);
  } else {
    Serial.println("SUCCESS");

    pox.setOnBeatDetectedCallback(onBeatDetected);
  }

  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

  // Register a callback for the beat detection

}
void loop() {
  server.handleClient();
  pox.update();
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();

  if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {

    Serial.print("BPM: ");
    Serial.println(BPM);

    Serial.print("SpO2: ");
    Serial.print(SpO2);
    Serial.println("%");

    Serial.println("*");
    Serial.println();

    tsLastReport = millis();
  }

}

void handle_OnConnect() {

  server.send(200, "text/html", SendHTML(BPM, SpO2));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(float BPM, float SpO2) {
  String ptr = "<!DOCTYPE html>";
  ptr += "<html lang='es'>";
  ptr += "<head>";
  ptr += "<meta charset='UTF-8'>";
  ptr += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  ptr += "<title>Pulse Oximeter ESP32 WebServer</title>";
  ptr += "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.7.2/css/all.min.css'>";
  ptr += "<style>";
  
  // Estilos generales
  ptr += "body {";
  ptr += "background-size: cover;";
  ptr += "background-repeat: no-repeat;";
  ptr += "background-position: center;";
  ptr += "margin: 0;";
  ptr += "font-family: 'Times New Roman', Times, serif, sans-serif;";
  ptr += "color: #333333;";
  ptr += "}";

  // Contenedor principal
  ptr += ".container {";
  ptr += "background-color: rgb(124, 200, 236);";
  ptr += "color: #000;";
  ptr += "margin: 100px auto;";
  ptr += "   font-size: 2.7rem;"; /* Tamaño de letra aumentado */
  ptr += "padding: 20px;";
  ptr += "max-width: 800px;";
  ptr += "border-radius: 15px;";
  ptr += "box-shadow: 0 0 15px rgba(0, 0, 0, 0.1);";
  ptr += "text-align: justify;";
  ptr += "}";

  ptr += ".p {";
  ptr += "   font-size: 3rem;"; /* Tamaño de letra aumentado */
  ptr += "}";

  // Estilos del encabezado
  ptr += ".header {";
  ptr += "text-align: center;";
  ptr += "background-color: rgba(0, 128, 128, 0.8);";
  ptr += "border-radius: 60px;";
  ptr += "}";

  ptr += ".header h1 {";
  ptr += "color: #fff;";
  ptr += "font-size: 36px;";
  ptr += "font-weight: bold;";
  ptr += "font-family: 'Garmond', sans-serif;";
  ptr += "margin: 0;";
  ptr += "}";

  ptr += ".header h3 {";
  ptr += "font-size: 4rem;"; /* Tamaño de letra más grande para el h3 */
  ptr += "color: #ddd;";
  ptr += "font-family: Arial, sans-serif;";
  ptr += "margin-top: 10px;";
  ptr += "}";

  // Estilos para los sensores
  ptr += ".sensor {";
  ptr += "font-size: 2.5rem;"; /* Tamaño de letra aumentado */
  ptr += "margin: 20px 0;";
  ptr += "display: flex;";
  ptr += "align-items: center;";
  ptr += "}";

  ptr += ".sensor i {";
  ptr += "margin-right: 20px;";
  ptr += "}";

  ptr += ".sensor-labels {";
  ptr += "font-weight: bold;";
  ptr += "}";

  ptr += "hr {";
  ptr += "border: none;";
  ptr += "height: 4px;";
  ptr += "background-color: #000;";
  ptr += "margin: 20px 0;";
  ptr += "}";

  // Estilos responsivos
  ptr += "@media (max-width: 494px) {";
  ptr += "#page { width: 100%; margin: 5px auto; }";
  ptr += ".box-full { width: 100%; margin: 10px auto; padding: 15px; }";
  ptr += "}";

  ptr += "@media (min-width: 495px) and (max-width: 980px) {";
  ptr += "#page { width: 465px; margin: 20px auto; }";
  ptr += ".box-full { width: 90%; }";
  ptr += "}";

  ptr += "@media (min-width: 981px) {";
  ptr += "#page { width: 930px; margin: auto; }";
  ptr += ".box-full { width: 800px; }";
  ptr += "}";

  ptr += "</style>";

  // Ajax para actualizar la página cada segundo
  ptr += "<script>";
  ptr += "setInterval(loadDoc, 1000);";
  ptr += "function loadDoc() {";
  ptr += "var xhttp = new XMLHttpRequest();";
  ptr += "xhttp.onreadystatechange = function() {";
  ptr += "if (this.readyState == 4 && this.status == 200) {";
  ptr += "document.body.innerHTML = this.responseText;";
  ptr += "}};";
  ptr += "xhttp.open('GET', '/', true);";
  ptr += "xhttp.send();";
  ptr += "}";
  ptr += "</script>";

  ptr += "</head>";
  ptr += "<body>";

  ptr += "<div id='page'>";
  ptr += "<div class='header'>";
  ptr += "<h3>Lector de Pulso y Niveles de Oxígeno</h3>";
  ptr += "</div>";

  ptr += "<div class='container'>";
  ptr += "<h3>Lectura del sensor</h3>";
  ptr += "<div class='box-full' align='left'>";

  // Lectura de BPM
  ptr += "<div class='p'>";
  ptr += "<p class='sensor'>";
  ptr += "<i class='fas fa-heartbeat' style='color:#cc3300'></i>";
  ptr += "<span class='sensor-labels'>Frecuencia cardíaca:_</span>";
  ptr += String((int)BPM);
  ptr += "<span class='sensor-labels'>_BPM</span>";
  ptr += "</p>";
  ptr += "</div>";
  ptr += "<hr>";

  // Lectura de SpO2
  ptr += "<div class='p'>";
  ptr += "<p class='sensor'>";
  ptr += "<i class='fas fa-burn' style='color:#f7347a'></i>";
  ptr += "<span class='sensor-labels'>SpO2:_</span>";
  ptr += String((int)SpO2);
  ptr += "<span class='sensor-labels'>_%</span>";
  ptr += "</p>";
  ptr += "</div>";

  ptr += "</div>";
  ptr += "</div>"; // container
  ptr += "</div>"; // page
  ptr += "</body>";
  ptr += "</html>";

  return ptr;
}

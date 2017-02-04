#include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h> // Библиотека для OTA-прошивки
#include "HTML_pages.h"

const char* ssid = "HP_LaserJet_1153"; //Имя точки доступа WIFI 
const char* password = "verylongwifipassword"; //пароль точки доступа WIFI 

ESP8266WebServer server(80);

const int led = 13;

const String str = "<!DOCTYPE html><html><head><title></title><meta charset='utf-8'></head><body><center><h1>Mechamics </h1></center><br><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><br><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><br><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><br><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><br><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><br><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><input type='button' value='1' style='width:300Px;height:200Px' onclick=\"location.href='mech?button=51';\"><br><a href='/game'> Game process </a><br><br><br><a href='/mech'>Mechanics</a><br><br><br><a href='/light'>Light</a></body></html>";
  
 

String readed = "";



 void handleRoot() {
//  digitalWrite(led, 1);
  server.send(200, "text/html", " Dracula quest. <br> <br> <br> You can control ______ <a href='/game'> Game process </a> ______ or ______ <a href='/mech'> Mechanics </a> ______ or ______ <a href='/light'> Light </a> <br><br><br><br><br><br><br><br><br><br> Site created by Internet of Things company: www.nicelight.ru");
//  digitalWrite(led, 0);
}


void handleMech() { // страница с механикой
  // обработчик кнопок
  if (server.hasArg("button")){
    if (server.arg("button") == "51") {
      Serial.print("wifiget51\n");
    server.send(200, "text/html", " Kartina otkrivaetsa... <br> <br> <br> You can control: <br><br><br> <a href='/game'> Game process </a> <br><br><br>__________ <a href='/mech'> Mechanics </a> <br><br><br>_____________________ <a href='/light'> Light </a> <br><br><br><br>");
    }
    if (server.arg("button") == "52") {
      Serial.print("wifiget52\n");
    server.send(200, "text/html", " Stop kartina <br> <br> <br> You can control: <br><br><br> <a href='/game'> Game process </a> <br><br><br>__________ <a href='/mech'> Mechanics </a> <br><br><br>_____________________ <a href='/light'> Light </a> <br><br><br><br>");
    }
    if (server.arg("button") == "53") {
      Serial.print("wifiget53\n");
    server.send(200, "text/html", " Kartina ZAKRIVAETSA... <br> <br> <br> You can control: <br><br><br> <a href='/game'> Game process </a> <br><br><br>__________ <a href='/mech'> Mechanics </a> <br><br><br>_____________________ <a href='/light'> Light </a> <br><br><br><br>");
    }
    if (server.arg("button") == "4") {
      Serial.write(4);
    }
    if (server.arg("button") == "5") {
      Serial.write(5);
    }
    if (server.arg("button") == "6") {
      Serial.write(6);
    }
    if (server.arg("button") == "7") {
      Serial.write(7);
    }
    if (server.arg("button") == "8") {
      Serial.write(8);
    }
    if (server.arg("button") == "9") {
      Serial.write(9);
    }
    if (server.arg("button") == "10") {
      Serial.write(10);
    }
  }

  // вывод страницы
 server.send(200, "text/html",html_page_mech); // эта страница описана как static const char в файле HTML_pages.h


}

void handleNotFound(){
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n"; // \n\n - принудтельный перевод строки
  message += "URI: ";
  message += server.uri(); // пропишет тот адрес на который пользователь хотел попасть
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
}

void setup(void){
  //pinMode(led, OUTPUT);
  //digitalWrite(led, 0);
  Serial.begin(19200); // скорость ограничиваю ибо под квесты Fantasmia прогу адаптирую
  
  // Подключение к точке доступа
  // WiFi.mode(WIFI_STA);
  // WiFi.begin("Brainy", "123456789qwertyPO");
  /*
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
   */

  WiFi.disconnect(); // завершим все wifi соединения
  WiFi.softAPdisconnect(); // отключаем точку доступа ( если вдруг успела сформироваться)
  WiFi.mode(WIFI_OFF); // отключаем вайфай физически
  delay(500);

  // поднимаем соединение со статическим адресом
  WiFi.mode(WIFI_STA);
  WiFi.config(IPAddress(192, 168, 1, 201), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0), IPAddress(192, 168, 1, 1));
  WiFi.begin(ssid, password);
  
  // ждем пока подключение произойдет
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }

    ArduinoOTA.setHostname("ESP8266-dracula"); // Задаем имя сетевого порта
 //  ArduinoOTA.setPassword((const char *)"0000"); // Задаем пароль доступа для удаленной прошивки
   ArduinoOTA.begin(); // Инициализируем OTA


  // Создание точки доступа
//  WiFi.mode(WIFI_AP);
 // WiFi.softAP("Fantasmia", "testquest");
 // WiFi.softAP(ssid, password);
 // Serial.println("AP opend");

  server.on("/", handleRoot);
// server.on("/game", handleGame);
 server.on("/mech", handleMech);
// server.on("/light", handleLight);

  server.on("/page2", [](){
    server.send(200, "text/plain", "page 2 also works as well");
  });

  
  server.onNotFound(handleNotFound);


  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
ArduinoOTA.handle(); // Всегда готовы к прошивке

  server.handleClient();

  if (Serial.available()) {
    readed = Serial.readString();
  }
}

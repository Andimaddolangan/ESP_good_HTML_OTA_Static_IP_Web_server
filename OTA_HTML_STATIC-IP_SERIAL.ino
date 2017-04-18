/* example for Arduino Witty board of ESP12
* ESP as wifi client connecting to router
* with static ip ( 192.168.1.241 in example)
* ESP making up Web server, with 
* front-end page in included file HTML_pages.h
* Also Serial parser in sketch for debuging
* OTA flashing allowed after first serial flashing
*/

      //BME280
      #include <Wire.h>
      #include <SPI.h>
      #include <Adafruit_Sensor.h>
      #include <Adafruit_BME280.h>
      #include <SerialFlow.h>
      #include <Adafruit_GFX.h>
      #include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h> // Библиотека для OTA-прошивки
#include "HTML_pages.h"

const char* ssid = "HP_LaserJet_1153"; //Имя точки доступа WIFI
const char* password = "verylongwifipassword"; //пароль точки доступа WIFI
ESP8266WebServer server(80);

// пины платты
const int LDR = A0;
const int BUTTON = 4;
const int RED = 15;
const int GREEN = 12;
const int BLUE = 13;


    #define BME_SCK 13
    #define BME_MISO 12
    #define BME_MOSI 11
    #define BME_CS 103
    
    #define SEALEVELPRESSURE_HPA (1000)
    //#define SEALEVELPRESSURE_HPA (1013.25)    //114 метров
    #define SEALEVELPRESSURE_HPA (1015.00) //128 метров

    Adafruit_BME280 bme; // I2C
    //Adafruit_BME280 bme(BME_CS); // hardware SPI
    //Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI
    //SerialFlow rd(&Serial); // RX, TX
    static const byte PACKET_SIZE = 1;
    static const byte VALUE_SIZE = 2;
    static const boolean SEPARATE_VALUES = true;


byte led_status = 1; // автомат для статусного светодиода
byte temperature_status = 1; // автомат опроса датчика BME280
byte mode_sm = 0; //автомат работы основной программы

unsigned int temperature_int = 0;

// time counters
unsigned long cur=0, ms_status_led = 0, ms_temperature = 0, ms1 = 0;

float temperature_float = 0;

//хэндлеры web сервера
void handleTemp() { // вывод температуры
  String str = "<!DOCTYPE html><html><head><title> Инкубатор Stavropol v1.0 </title><meta charset='utf-8'></head>";
  str += "<body><center><h1> Отображения состояния.</h1></center><br>";
  str += "<br><br><br>Текущая Температура: ";
  str += current_float_temp;
  str += " *C ";
  str += "<br><br><br><br><br><br>";
  str += "<a href='/temp'>Нажать сюда, чтобы обновить данные</a>";
  str += "<br><br><br></body></html>";
  server.send(200, "text/html", str);
}// handle temp

void handleRoot() {
 // вывод страницы
 server.send(200, "text/html",html_main_page); // эта страница описана как static const char в файле HTML_pages.h
}//handle root

void handleLeds() {
  if (server.hasArg("red")) {
    if (server.arg("red") == "1") {
      digitalWrite(RED, 1);
    server.send(200, "text/html",html_main_page); // эта страница описана как static const char в файле HTML_pages.h
    }
    if (server.arg("red") == "0") {
      digitalWrite(RED, 0);
    server.send(200, "text/html",html_main_page); // эта страница описана как static const char в файле HTML_pages.h
    }
  }//has.arg red
  if (server.hasArg("green")) {
    if (server.arg("green") == "1") {
      digitalWrite(GREEN, 1);
    server.send(200, "text/html",html_main_page); // эта страница описана как static const char в файле HTML_pages.h
    }
    if (server.arg("green") == "0") {
      digitalWrite(GREEN, 0);
    server.send(200, "text/html",html_main_page); // эта страница описана как static const char в файле HTML_pages.h
    }
  }//has.arg green
  if (server.hasArg("blue")) {
    if (server.arg("blue") == "1") {
      digitalWrite(BLUE, 1);
    server.send(200, "text/html",html_main_page); // эта страница описана как static const char в файле HTML_pages.h
    }
    if (server.arg("blue") == "0") {
      digitalWrite(BLUE, 0);
    server.send(200, "text/html",html_main_page); // эта страница описана как static const char в файле HTML_pages.h
    }
  }//has.arg blue
}//handle leds

void handleNotFound() {
  //digitalWrite(led, 1);
  String message = "File Not Found\n\n"; // \n\n - принудтельный перевод строки
  message += "URI: ";
  message += server.uri(); // пропишет тот адрес на который пользователь хотел попасть
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  //digitalWrite(led, 0);
}

void setup(void) {

    pinMode(13, OUTPUT);
    Serial.begin(115200);
    
    digitalWrite(13, 0);
    delay(50);
    digitalWrite(13, 1);
    delay(200);
    digitalWrite(13, 0);
    delay(50);
    digitalWrite(13, 1);
    delay(200);
    digitalWrite(13, 0);
    delay(50);
    digitalWrite(13, 1);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
   Wire.begin(21, 22); //21 - sda, 22 - scl. по другому нельзя
   Serial.println("Software Wire bus started on pins ");
   delay(50);  
   bool status;
    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    Serial.println(F("BME280 started sucsessfully."));
    Serial.println(" ");
    Serial.println(" ");
    Serial.println(" ");
    
    
  WiFi.disconnect(); // завершим все wifi соединения
  WiFi.softAPdisconnect(); // отключаем точку доступа ( если вдруг успела сформироваться)
  WiFi.mode(WIFI_OFF); // отключаем вайфай физически
  delay(500);
  Serial.println("wifi force disconnecting...");
  // поднимаем соединение со статическим адресом
  WiFi.mode(WIFI_STA);
  WiFi.config(IPAddress(192, 168, 1, 241), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0), IPAddress(192, 168, 1, 1));
  WiFi.begin(ssid, password);
  Serial.print("Connecting to wifi: ");
  Serial.print(ssid);
  Serial.print("  with pass:");
  Serial.println(password);
  
  // ждем пока подключение произойдет
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("  Connected with static ip: ");
  Serial.println(WiFi.localIP());
  
  ArduinoOTA.setHostname("esp_Witty_BME280_test_ip241"); // Задаем имя сетевого порта
  //  ArduinoOTA.setPassword((const char *)"0000"); // Задаем пароль доступа для удаленной прошивки
  ArduinoOTA.begin(); // Инициализируем OTA
  Serial.print("OTA HostName: ");
  Serial.println("esp_Witty_BME280_test_ip241");

  server.on("/leds", handleLeds); // управление
  server.on("/temp", handleTemp); // вывод температуры
  server.on("/", handleRoot);
  server.on("/page2", []() {
    server.send(200, "text/plain", "page 2 also works well");
  });

  server.onNotFound(handleNotFound);
  server.begin();
Serial.println("HTTP server started");

    
    bool status = 0; // инициализируем датчик bme
    // default settings
    
    while(!status) {
      status = bme.begin();
      Serial.println("Could not find a valid BME280 sensor, check wiring!");
      delay(300);
    }
    

}//setup

void loop(void) {
  cur = millis();
  ArduinoOTA.handle(); // Всегда готовы к прошивке
  server.handleClient();
  delay(1);
  cur = millis();
  
  switch(led_status){ 
    //case 1-5: мигаем раз в секунду
    case 1:
      if((cur - ms_status_led) > 1950){
        ms_status_led = cur;
        digitalWrite(13, 0);
        led_status = 5; //SWITCH
      }//if ms
    break;
    case 5:
      if((cur - ms_status_led) > 50){
        ms_status_led = cur;
        digitalWrite(13, 1);
        led_status = 1; //SWITCH
      }//if ms
    break;
  }//switch led status

switch(temperature_status){
    // вывод в серийник
    case 1:
    Serial.print("Temperature = ");
    Serial.print(bme.readTemperature());
    Serial.print(" *C\t");

    Serial.print("Pressure = ");

//    Serial.print(bme.readPressure() / 100.0F);
//    Serial.print(" hPa\t");

    Serial.print(bme.readPressure() / 133.3F);
    Serial.print(" mm. rt. st.\t");


    Serial.print("Approx. Altitude = ");
    Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
    Serial.print(" m\t");

    Serial.print("Humidity = ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    ms_temperature = cur;
    temperature_status = 3;
    break;
    // пауза в секунду
    case 3:
      if((cur - ms_temperature) > 1000){
        ms_temperature = cur;
        temperature_status = 1; //SWITCH
      }//if ms
    break;
  }//switch temperature status  
  
  switch(mode_sm){
    case 0:
    break;
    //состояния 1-2 мигалка
    case 1:
    if((cur - ms1) > 800){
      digitalWrite(13, 1);//do something
      ms1 = cur;
      mode_sm = 2; //SWITCH
    }//if ms
    break;
    case 2:
    if((cur - ms1) > 200){
      digitalWrite(13, 0);//do something
      ms1 = cur;
      mode_sm = 1; //SWITCH
    }//if ms
    break;
  }//switch effects
}//loop


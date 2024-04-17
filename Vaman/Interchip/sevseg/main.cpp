#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "esp32_eoss3_spi.h"

#define GPIO_OUTPUT_VAL_REG 0x40021004
#define GPIO_OUTPUT_DIR_REG 0x40021008

#define PIN_LED1 18
#define PIN_LED2 21
#define PIN_LED3 22
#define PIN_LED4 23
#define PIN_LED5 24
#define PIN_LED6 25
#define PIN_LED7 26

AsyncWebServer server(80);

const char* ssid = "Oneplus";
const char* password = "12345679";

const char* PARAM_STATE = "state";
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
<html>
<head>
  <title>Sevenseg</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
</head>
<body>
  <h1>Seven Segment Display</h1>
  <form action="/led" method="POST">
    <button type="submit" name="state" value="0">Value 0</button><br>
    <button type="submit" name="state" value="1">Value 1</button><br>
    <button type="submit" name="state" value="2">Value 2</button><br>
    <button type="submit" name="state" value="3">Value 3</button><br>
    <button type="submit" name="state" value="4">Value 4</button><br>
    <button type="submit" name="state" value="5">Value 5</button><br>
    <button type="submit" name="state" value="6">Value 6</button><br>
    <button type="submit" name="state" value="7">Value 7</button><br>
    <button type="submit" name="state" value="8">Value 8</button><br>
    <button type="submit" name="state" value="9">Value 9</button><br>
  </form>
</body>
</html>
)rawliteral";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
  esp32_eoss3_spi_init();
  uint32_t dirval = (1 << PIN_LED1) | (1 << PIN_LED2) | (1 << PIN_LED3) | (1 << PIN_LED4) | (1 << PIN_LED5) | (1 << PIN_LED6) | (1 << PIN_LED7);
  uint32_t gpioval = 0;
  esp32_eoss3_spi_ahb_write(GPIO_OUTPUT_DIR_REG, (uint8_t *)&dirval, 4);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(2000);
  }
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/led", HTTP_GET, [&gpioval](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  server.on("/led", HTTP_POST, [&gpioval](AsyncWebServerRequest *request) {
    int params = request->params();
    gpioval = 0; // Reset the GPIO value to turn off all LEDs
    for(int i=0;i<params;i++){
      AsyncWebParameter* p = request->getParam(i);
      if(p->isPost()){
        if (p->name() == PARAM_STATE) {
          uint8_t option = p->value().toInt();
          switch(option) {
          case 0:
            gpioval = (1 << PIN_LED7);
            break;
          case 1:
            gpioval = (1 << PIN_LED1) | (1 << PIN_LED4) | (1 << PIN_LED5) | (1 << PIN_LED6) | (1 << PIN_LED7);
            break;
          case 2:
            gpioval = (1 << PIN_LED3) | (1 << PIN_LED6);
            break;
          case 3:
            gpioval = (1 << PIN_LED5) | (1 << PIN_LED6);
            break;
          case 4:
            gpioval = (1 << PIN_LED1) | (1 << PIN_LED4) | (1 << PIN_LED5);
            break;
          case 5:
            gpioval = (1 << PIN_LED2) | (1 << PIN_LED5);
            break;
          case 6:
            gpioval = (1 << PIN_LED1) | (1 << PIN_LED2);
            break;
          case 7:
            gpioval = (1 << PIN_LED4) | (1 << PIN_LED5) | (1 << PIN_LED6) | (1 << PIN_LED7);
            break;
          case 8:
            gpioval = 0;
            break;
          case 9:
            gpioval = (1 << PIN_LED4) | (1 << PIN_LED5);
            break;
          default:
    // Handle unknown cases
            break;

          }
        }
      }
    }
    // Write the GPIO value to turn on/off LEDs
    esp32_eoss3_spi_ahb_write(GPIO_OUTPUT_VAL_REG, (uint8_t *)&gpioval, 4);
    request->send(200, "text/html", index_html);
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {}


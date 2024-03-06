//----------------------Skeleton Code--------------------//
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

//    Can be client or even host   //
#ifndef STASSID
#define STASSID "Oneplus" // Add your network credentials
#define STAPSK "12345679"
#endif
#define LED_PIN 5
const char *ssid = STASSID;
const char *password = STAPSK;

void OTAsetup()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.begin();
}

void OTAloop()
{
  ArduinoOTA.handle();
}

//-------------------------------------------------------//

void setup()
{
  OTAsetup();
  pinMode(2, INPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);

  //-------------------//
  // Custom setup code //
  //-------------------//
}

void loop()
{
  OTAloop();
  int P = 1, Q = 1, X = 0, Y = 0;
  if (digitalRead(2) == HIGH)
  {
    X = !(Y && P);
    Y = !(X && Q);
    if (X)
    {
      digitalWrite(4, HIGH);
    }
    if (Y)
    {
      digitalWrite(5, HIGH);
    }
  }
  if (digitalRead(2) == LOW)
  {
    Y = !(X && Q);
    X = !(Y && P);
  }
  if (X)
  {
    digitalWrite(4, HIGH);
  }
  if (Y)
  {
    digitalWrite(5, HIGH);
  }
  delay(1000);

  // put your main code here, to run repeatedly:
}

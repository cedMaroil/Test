/*
  Sample application to post data from ESP8266 module
  to the cloud for storage, visualization and analysis.
  Please check repository below for details
  Repository: https://github.com/krzychb/EspToCloud
  Version: Emoncms
  File: EspToCloud-Emoncms.ino
  Revision: 0.1.1
  Date: 18-Jul-2016
  Author: krzychb at gazeta.pl
  Copyright (c) 2016 Krzysztof Budzynski. All rights reserved.
  This application is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.
  This application is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.
  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "LedBlinking.h"


#ifdef ESP8266
extern "C" {
#include "user_interface.h"
}
#endif

const char* ssid = "WiFi-2.4-F3B9";
const char* password = "5CE0032085";

#define UPDATE_PERIOD 10000
#define TIMEOUT_PERIOD 1000
#define DATA_HOST "emoncms.org"
#define API_KEY "ac1533354f44f2b84582008a2ccc1724"
#define STATUSLED 2

void keypadEventCed(KeypadEvent key);

LedBlinking ledBlinking;
LedBlinking receiveData;
String serialData;

void logToEmoncms(void)
{
  WiFiClient client;
  static long dataLogTimer;
  static long transmissionAttempts;
  static long replyTimeouts;
  static long connectionFailures;
  static long postingTime = -1;

  // is it the time for next data logging?
  if (millis() - dataLogTimer < UPDATE_PERIOD)
  {
    return;
  }
  dataLogTimer = millis();
  transmissionAttempts++;

  Serial.printf("DATALOG > %s > ", DATA_HOST);

  // connect to the host
  if (!client.connect(DATA_HOST, 80))
  {
    Serial.print("Connection failure!");
    connectionFailures++;
    return;
  }

  Serial.print("Sending > ");

  // send the data to the host
  client.print(
  String("GET http://") + DATA_HOST +
        "/input/post.json?json={" +
          "EspCloud_UpTime:" + String (dataLogTimer / 60000) +
          ",EspCloud_HeapSize:" + String (system_get_free_heap_size()) +
          ",EspCloud_TransmissionAttempts:" + String (transmissionAttempts) +
          ",EspCloud_ReplyTimeouts:" + String (replyTimeouts) +
          ",EspCloud_ConnectionFailures:" + String (connectionFailures) +
          ",EspCloud_PostingTime:" + String (postingTime) +
          "}&apikey=" + API_KEY + "\r\n" +
        "Connection: close\r\n\r\n");

  // wait for reply from the host
  while (!client.available())
  {
    if (millis() - dataLogTimer > TIMEOUT_PERIOD)
    {
      Serial.println("Reply timeout!");
      replyTimeouts++;
      postingTime = -1;
      client.stop();
      return;
    }
  }

  // show reply from the host
  while (client.available())
  {
    Serial.write(client.read());
  }
  Serial.print(" > Done");
  client.stop();

  // show time to log the data
  postingTime = millis() - dataLogTimer;
  Serial.printf(" in %d ms\n", postingTime);
}


void setup(void)
{
    ledBlinking = LedBlinking(STATUSLED, 700, 400);
    receiveData = LedBlinking(STATUSLED, 200, 200, 3);
  Serial.begin(115200);
  Serial.println();
  Serial.println("EspToCloud-Emoncms 0.1.1");

  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  Serial.printf("Data are posted to %s every %d seconds\n", DATA_HOST, UPDATE_PERIOD/1000);
  Serial.println();
}


void loop(void)
{
  ledBlinking.UpdateLed();
  receiveData.UpdateLed();
  logToEmoncms();

  ledBlinking.addEventListener(keypadEventCed);
  /*
  while(Serial.available() > 0){
      receiveData.Blink();
      Serial.println("coucou");
      //if(Serial.read() == '\r'){
      //    Serial.println(serialData);
      //    serialData = "";
      //}
      serialData = String(Serial.readString());
      Serial.println(serialData);
  }
  */
  
}
void keypadEventCed(KeypadEvent key){
  Serial.println(key);
}

/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ESP8266WiFi.h>
//#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>

#include "wifi.h" // define SSID and PASSWORD
#include "html.h"

const char* ssid = SSID;
const char* password = PASSWORD;
ESP8266WebServer server(80);
html rootHtml;

const int motorA = D1;
const int motorB = D2;
const int dirA = D3;
const int dirB = D4;
const int pinRelay0 = D0;

int posX = 0;
int posY = 0;

WebSocketsServer webSocket = WebSocketsServer(81);
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocket.sendTXT(num, "Connected");
            }
            break;
        case WStype_TEXT:
        {
          Serial.printf("[%u] get Text: %s\n", num, payload);
          const char* p = (const char*) payload;
          String s( p );
          //Serial.printf("Text : %s\n", s.substring(2).c_str());
          int value = s.substring(2).toInt();
          //Serial.printf("Int : %i\n", value);
          if(s.substring(0,1)=="X")
            posX = value;
          if(s.substring(0,1)=="Y")
            posY = value;

          if(posX>1023)
            posX = 1023;
          else if(posX<-1023)
            posX = -1023;
          if(posY>1023)
            posY = 1023;
          else if(posY<-1023)
            posY = -1023;

// if X is negative, motorA speed reduced, and if positive, motorB speed reduced
          int speedL = -posX + posY;
          int speedR = posX + posY;
          if(speedL>1023) speedL = 1023;
          if(speedL<-1023) speedL = -1023;
          if(speedR>1023) speedR = 1023;
          if(speedR<-1023) speedR = -1023;
        

          Serial.printf("Send left %i (-%i+%i), right %i (%i+%i)\n", speedL, posX, posY, speedR, posX, posY);

          digitalWrite( dirA, (speedL>0) ? 1 : 0 );
          digitalWrite( dirB, (speedR>0) ? 1 : 0 );
          analogWrite( motorA, abs(speedL) );
          analogWrite( motorB, abs(speedR) );
        }
        break;
        case WStype_BIN:
            Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
            hexdump(payload, lenght);

            // send message to client
            // webSocket.sendBIN(num, payload, lenght);
            break;
    }

}

void handleRoot() {
  if( server.args() > 0 )
  {
    String argName_0 = server.argName(0);
    //Serial.println(argName_0);
    if(argName_0 == "d")
    {
      if(server.arg(0) == "stop")
      {
        analogWrite ( motorA, 0 );
        analogWrite ( motorB, 0 );
      }
      if(server.arg(0) == "Fn1")
      {
        digitalWrite ( pinRelay0, 1);
        Serial.println ( "pinRelay0 started" );
      }
      if(server.arg(0) == "Fn2")
      {
        digitalWrite ( pinRelay0, 0);
        Serial.println ( "pinRelay0 stopped" );
      }
    }
    else {
    }
  }

  int rootPageSize = rootHtml.size();
  char temp[rootPageSize];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;

  Serial.print ( "HTML size ");
  Serial.println( rootPageSize );
	snprintf ( temp, rootPageSize, rootHtml.getIndexPage().c_str(),	hr, min % 60, sec % 60 );
	server.send ( 200, "text/html", temp );
}

void handleScript() {
  int PageSize = rootHtml.scriptSize();
  char temp[PageSize];
  snprintf (temp, PageSize, rootHtml.getJsPage().c_str());
  server.send ( 200, "text/html", temp );
}

void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}

	server.send ( 404, "text/plain", message );
}

void setup ( void ) {
	pinMode ( motorA, OUTPUT );
  pinMode ( motorB, OUTPUT );
  pinMode ( dirA, OUTPUT );
  pinMode ( dirB, OUTPUT );
  pinMode ( dirB, OUTPUT );
  pinMode ( pinRelay0, OUTPUT);

	analogWrite ( motorA, LOW );
  analogWrite ( motorB, LOW );
  digitalWrite ( dirA, LOW );
  digitalWrite ( dirB, LOW );
	Serial.begin ( 115200 );
	WiFi.begin ( ssid, password );
	Serial.println ( "" );

	// Wait for connection
	while ( WiFi.status() != WL_CONNECTED ) {
		delay ( 500 );
		Serial.print ( "." );
	}

	Serial.println ( "" );
	Serial.print ( "Connected to " );
	Serial.println ( ssid );
	Serial.print ( "IP address: " );
	Serial.println ( WiFi.localIP() );

	if ( MDNS.begin ( "catbot" ) ) {
		Serial.println ( "MDNS responder started" );
	}

	server.on ( "/", handleRoot );
  server.on ( "/scripts/mouse.js", handleScript );
	server.onNotFound ( handleNotFound );
	server.begin();
	Serial.println ( "HTTP server started" );

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println ( "Websocket server started" );
}

void loop ( void ) {
	server.handleClient();
  webSocket.loop();
}

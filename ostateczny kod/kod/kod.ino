
#include <Wire.h>                  // Include Wire library (required for I2C devices)



#include <SoftwareSerial.h>

#include "pitches.h" //header with frequencies for respective notes
#include "durations.h" //header with duration of notes (denominator of duration for a note)
#include "melody.h"


int val = 0;
const int stepsPerRevolution = 200;
String buzzer_message = "OFF";
boolean state=false;
#define led_pin1 13
#define led_pin2 11
#define led_pin3 12






double temperatura;
int LM35 = 0;

SoftwareSerial esp8266(3, 4);
#define DEBUG true




void onInterrupt() {
    Serial.println("ON");
    state=!state;
}

String esp8266Serial(String command, const int timeout, boolean debug)
{
  String response = "";
  esp8266.print(command); //prints command inside serial
  long int time = millis();
  while ( (time + timeout) > millis()) //waits specified amount of time (usually about 5 seconds)
  {
    while (esp8266.available()) //checking if the connection is established and 
    {                              // and the device is responding
      char c = esp8266.read();
      response += c;               // recording response
    }
  }
  if (debug)
  {
    Serial.print(response);    //in case of debug mode prints response on SerialMonitor
  }
  return response; 
}


void setup() {

  Serial.begin(9600);
  esp8266.begin(115200); //Baud rate for communicating with ESP8266. 
  esp8266Serial("AT+RST\r\n", 5000, DEBUG); // Reset the ESP8266
  esp8266Serial("AT+CWMODE=1\r\n", 5000, DEBUG); //Set station mode Operation

  esp8266Serial("AT+CWJAP=\"arista_home\",\"fiesta11d\"\r\n", 5000, DEBUG);//Enter your WiFi network's SSID and Password.
  while (!esp8266.find("OK"))
  {
  }
  esp8266Serial("AT+CIFSR\r\n", 5000, DEBUG);//You will get the IP Address of the ESP8266 from this command.
  esp8266Serial("AT+CIPMUX=1\r\n", 5000, DEBUG);
  esp8266Serial("AT+CIPSERVER=1,80\r\n", 5000, DEBUG);
  //if (! ourclock.isrunning()) { //checks if clock is plugged and running
    Serial.println("Clock is not running! :/");

}

void loop() {


    if (esp8266.available())
    {

      if (esp8266.find("+IPD,"))
      {
        String msg;
        esp8266.find("HTTP/1.1");
        msg = esp8266.readStringUntil(';');
         Serial.println(msg);
        int firstAppresant = msg.indexOf('&');
        String command1 = msg.substring(0, firstAppresant);
        String command = msg.substring(firstAppresant + 1);
         firstAppresant = command.indexOf('&');
        String command2 = command.substring(0, firstAppresant);
         command = command.substring(firstAppresant + 1);
         firstAppresant = command.indexOf('&');
        String command3 = command.substring(0, firstAppresant);
        String commandbuzzer = command.substring(firstAppresant + 1);
       
        String led1_message = command1.substring(5);
        String led2_message = command2.substring(5);
        String led3_message = command3.substring(5);
        buzzer_message = commandbuzzer.substring(7);



        if (!DEBUG)
        {
          Serial.println(command1);//Must print "led"
          Serial.println(command2);//Must print "ON" or "OFF"
          Serial.println(led1_message);
          Serial.println(led2_message);
          Serial.println(buzzer_message);
        }
        delay(100);


        
      }
    }
  }

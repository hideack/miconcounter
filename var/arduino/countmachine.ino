#include <stdio.h>
#include <SPI.h>
#include <Ethernet.h>

#define LED_PIN 9
#define SENSOR0 0
#define SENSOR1 1
 
byte mac[] = {0x90,0xA2,0xDA,0x0E,0x9B,0x8D}; // イーサシールドのMacアドレス記入
char hostname[] = "miconcounter-hideack.sqale.jp";

char serverName[256];
char httpHeaderHost[256];

EthernetClient client;
 
void setup()
{
    pinMode(LED_PIN, OUTPUT);
    pinMode(SENSOR0, INPUT);
    pinMode(SENSOR1, INPUT);
    
    sprintf(serverName,     "%s", hostname);
    sprintf(httpHeaderHost, "Host: %s", hostname);
    
    Serial.print("Target host:");
    Serial.println(hostName);
    
    Serial.begin(9600);
    if (Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        for (;;);
    }
    Serial.print("IP address: ");
    for (byte n=0; n<4; n++) {
        Serial.print(Ethernet.localIP()[n], DEC);
        Serial.print(".");
    }  
    Serial.println();
    delay(1000);
}
 
void loop()
{
    int ultrasonicVal = 0;
    int irVal = 0;
    
    ultrasonicVal = analogRead(SENSOR0);
    irVal = analogRead(SENSOR1);
    digitalWrite(LED_PIN, LOW);
    
    if (irVal > 150 && ultrasonicVal < 75) {
      digitalWrite(LED_PIN, HIGH);
      Serial.print("Ultrasonic:");
      Serial.print(ultrasonicVal, DEC);
      Serial.print("\n");
      Serial.print("IR:");
      Serial.print(irVal, DEC);
      Serial.print("\n");

      Serial.println("connecting..."); 
      if (client.connect(serverName, 80)) {
          Serial.println("connected");
          client.println("GET /count/aaa HTTP/1.1");  // カウントAPI指定
          client.println("User-Agent: aruduino");
          client.println(httpHeaderHost);
          client.println();
      } else {
          Serial.println("connection failed");
      }
    
      if (client.available()) {
          char c = client.read();
          Serial.print(c);
          client.stop();
          delay(500);
      }
    }
}


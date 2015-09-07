#include <SPI.h>
#include <UIPEthernet.h>                                  // UIP Ethernet Library to support ENC28J60 ethernet module. 
#include <PubSubClient.h>                                 // MQTT publisher/subscriber client library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>                            // i2C converter compatible library

LiquidCrystal_I2C lcd(0x27,16,2);

byte mac[]      = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED }; // Set the mac address for the arduino nano mcu 
//byte server[] = { xxx, xxx, xxx, xxx };                  // Set MQTT broker server IP address
char server[]   = "xxx.xxxxxxx.xxx";                       // Set MQTT broker server domain address 
byte ip[]       = { xxx, xxx, xxx, xxx };                  // Set the IP address for the arduino nano mcu 

int LED = 6;
String msgString;

void callback(char* topic, byte* payload, unsigned int length) {
  
  // handle message arrived
  char message_buff[100];
  int i;
  
  for(i=0; i<length; i++) {
    message_buff[i] = payload[i];
  }
  
  message_buff[i] = '\0';
  msgString = String(message_buff);
  Serial.println("message arrived: " + msgString);  // Serial output of received callback payload

  if(msgString == "0")
  {
    			
    digitalWrite(LED, LOW);             // If message payload received is 0, LED is turned off
    lcd.clear();                        // Clear previously printed values in the LCD
    lcd.print("D6 : OFF");              // Print The Message on LCD
    lcd.setCursor(0,1);
    
  }
  else if (msgString == "1")
  {
    
    digitalWrite(LED, HIGH);   		      // If message payload received is 1, LED is turned on
    lcd.clear();			                  // Clear previously printed values in the LCD
    lcd.print("D6 : ON");		            // Print The Message on LCD
    lcd.setCursor(0,1);                 // Set top cursor position on 16x2 LCD module
    
  } else {

    lcd.clear();			                  // Clear previously printed values in the LCD
    lcd.print("Device Status");         // Print Device Status message on LCD
    lcd.setCursor(0,1);                 // set top cursor position on 16x2 LCD module
    lcd.print(msgString);		            // Print The message payload on LCD
    lcd.setCursor(1,1);                 // Set top cursor position on 16x2 LCD module

  }

}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);


void setup()
{
  
  Serial.begin(9600);
  pinMode(LED, OUTPUT);                     // set LED connected pin to OUTPUT mode
  
  lcd.init();                     	        // Initialize the LCD
  lcd.backlight();


  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient")) {
     client.publish("topic","connected");   // publish ethernet connected status to MQTT topic
     client.subscribe("topic");             // Subscribe to a MQTT topic
  }

}

void loop()
{
  client.loop();
}

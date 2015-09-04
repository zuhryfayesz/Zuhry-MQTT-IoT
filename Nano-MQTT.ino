#include <SPI.h>
#include <UIPEthernet.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // i2C converter compatible library

LiquidCrystal_I2C lcd(0x27,16,2);

byte mac[]      = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
//byte server[] = { xxx, xxx, xxx, xxx };
char server[]   = "xxx.xxxxxxx.xxx";
byte ip[]       = { xxx, xxx, x, xxx };

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
  Serial.println("message arrived: " + msgString);

  if(msgString == "0")
  {
    			
    digitalWrite(LED, LOW);		//if message received is 0, LED is turned off
    lcd.clear();			// Clear previously printed values in the LCD
    lcd.print("D6 : OFF");		// Print The Message on LCD
    lcd.setCursor(0,1);
    
  }
  else if (msgString == "1")
  {
    
    digitalWrite(LED,HIGH);   		//if message received is 1, LED is turned on
    lcd.clear();			// Clear previously printed values in the LCD
    lcd.print("D6 : ON");		// Print The Message on LCD
    lcd.setCursor(0,1);
    
  } else {

    lcd.clear();			// Clear previously printed values in the LCD
    lcd.print("Device Status");
    lcd.setCursor(0,1);
    lcd.print(msgString);		// Print The Message on LCD
    lcd.setCursor(1,1);

  }

}

EthernetClient ethClient;
PubSubClient client(server, 1883, callback, ethClient);


void setup()
{
  Serial.begin(9600);
  pinMode(LED, OUTPUT);
  // light should be off
  lcd.init();                     	// Initialize the LCD
  lcd.backlight();


  Ethernet.begin(mac, ip);
  if (client.connect("arduinoClient")) {
     client.publish("topic","connected");
     client.subscribe("topic");
  }

}

void loop()
{
  client.loop();
}

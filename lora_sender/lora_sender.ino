/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/ttgo-lora32-sx1276-arduino-ide/
*********/

//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "./variables.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(vbatPin, INPUT);
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.ssd1306_command(0x81);
  display.ssd1306_command(0x01);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER ");
  display.display();
  
  Serial.println("LoRa Sender Test");

  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  
  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSpreadingFactor(spreadingFactor);
  LoRa.setTxPower(txPower);

  Serial.println("LoRa Initializing OK!");
  display.setCursor(0,10);
  display.print("LoRa Initializing OK!");
  display.display();
  delay(1000);
}

void loop() {

  VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
  sendMessage();
  draw();

  msgCount++;
  
  delay(7000);
}

void sendMessage() {
  LoRa.beginPacket();                   // start packet
  String outgoing = "";
  outgoing += String(myPlayerID) + message + String(msgCount);
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  Serial.print("message sent :");
  Serial.println(outgoing);
  LoRa.end();
  LoRa.sleep();
}

void draw(){
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("LORA SENDER");
  display.setCursor(0,20);
  display.setTextSize(1);
  display.print("LoRa packet sent.");
  display.setCursor(0,30);
  display.print("Message:");
  display.setCursor(60,30);
  display.print(message);
  display.setCursor(0,40);
  display.print("Counter:");
  display.setCursor(60,40);
  display.print(msgCount);
  display.setCursor(100,56);
  display.print(VBAT); 
  display.display();
  delay(3000);
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

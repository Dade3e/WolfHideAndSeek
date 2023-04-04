//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "./variables.h"
#include "./icons.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

int sogliaRSSI = -60;

void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(vbatPin, INPUT);

  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }

  //display.ssd1306_command(SSD1306_DISPLAYON);
  display.ssd1306_command(0x81);
  display.ssd1306_command(0x01);
  display.setTextColor(WHITE);
  //display.dim(true);

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

  Serial.println("Setup OK!");
}


void loop() {
  attesa_msg_multi();
  if((millis()-attesa)>1000){
    attesa = millis();
    VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
    schermata_recv();
  }
}

void schermata_recv(){
  VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
  display.clearDisplay();

  if(message_id != -1){
    display.setTextSize(3);
    display.setCursor(5,15);
    display.print(message_id);
    display.print(":");
    display.print(message_rssi);
  }
  
  display.setCursor(0,0);
  //display.print("Id:");
  //display.setCursor(20,0);
  //display.print(myPlayerID);
  display.setTextSize(1);
  display.setCursor(5,56);
  display.print((millis()-lastRecvTime)/1000);
  display.setTextSize(1);
  display.setCursor(100,56);
  display.print(VBAT);
  display.display();
     
}

void schermata_recv_array(){
  VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
  display.clearDisplay();
  

  int k=0;
  for(int i=0;i<10;i++){
    if(senders[i] != 0){
      display.setTextSize(1);
      display.setCursor(8 + k*20,25);
      display.print(senders[i]);
      display.setTextSize(1);
      display.setCursor(k*20,40);
      display.print(senders_rssi[i]);
      k++;      
    }
  }
  
  display.setCursor(0,0);
  //display.print("Id:");
  //display.setCursor(20,0);
  //display.print(myPlayerID);
  display.setTextSize(1);
  display.setCursor(0,56);
  display.print((millis()-lastRecvTime)/1000);
  display.setTextSize(1);
  display.setCursor(100,56);
  display.print(VBAT);
  display.display();
     
}

void attesa_msg(){
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.println("");
    Serial.println("Received packet!");
    // read the packet
    message_recv = "";
    while (LoRa.available()) {
      message_recv += (char)LoRa.read();
    }
    int rssi = LoRa.packetRssi();
    // print the Packet and RSSI
    Serial.print("message_recv:\t");
    Serial.println(message_recv);
    Serial.print("rssi:\t\t");
    Serial.println(rssi);
    int id = (message_recv[0] - '0');
    if(id == 0){
      message_id = id;
      message_rssi = rssi;
      message = message_recv.substring(1);
      Serial.print("id:\t\t");
      Serial.println(id);
      Serial.print("message:\t");
      Serial.println(message);
      senders[id] = id;
      senders_rssi[id] = rssi;
      
      lastRecvTime = millis();
    } 
  }
}

void attesa_msg_multi(){
  int packetSize = LoRa.parsePacket();
  
  if (packetSize) {
    Serial.println("");
    Serial.println("Received packet!");
    // read the packet
    message_recv = "";
    while (LoRa.available()) {
      message_recv += (char)LoRa.read();
    }
    int rssi = LoRa.packetRssi();
    // print the Packet and RSSI
    Serial.print("message_recv:\t");
    Serial.println(message_recv);
    Serial.print("rssi:\t\t");
    Serial.println(rssi);
    int id = (message_recv[0] - '0');
    senders[id] = id;
    senders_rssi[id] = rssi;

    message_id = id;
    message_rssi = rssi;
    message = message_recv.substring(1);

    lastRecvTime = millis(); 
  }
}

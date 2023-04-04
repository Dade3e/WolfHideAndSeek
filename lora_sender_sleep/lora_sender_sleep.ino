//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "./variables.h"


#define TIME_TO_SLEEP 7 /* Duration ESP32 will go to sleep (seconds) */
#define TIME_UP 2 /* Duration ESP32 stays up (seconds) */

RTC_DATA_ATTR unsigned long msgCounter = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setup()
{
  msgCounter++;
  Serial.begin(115200);
  pinMode(vbatPin, INPUT);
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  initOLED();

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

  sendMessage(msgCounter);

  goToSleep(TIME_TO_SLEEP);
  return;
}

void loop()
{
  //non entra
  Serial.println("This will never be printed");
}

void goToSleep(int sleepSeconds) {
  LoRa.sleep();

  display.ssd1306_command(SSD1306_DISPLAYOFF);
  delay(100);

  pinMode(SCK, INPUT);
  pinMode(MISO, INPUT);
  pinMode(MOSI, INPUT);
  pinMode(SS, INPUT);
  pinMode(RST, INPUT);
  pinMode(DIO0, INPUT);
  pinMode(OLED_SDA, INPUT);
  pinMode(OLED_SCL, INPUT);
  pinMode(vbatPin, INPUT);

  Serial.printf("Deep sleeping for %d seconds\n", sleepSeconds);
  esp_sleep_enable_timer_wakeup(sleepSeconds * 1000000);
  Serial.flush();
  esp_deep_sleep_start();

  Serial.println("This will never be printed");
}


void initOLED()
{
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.ssd1306_command(SSD1306_DISPLAYON);
  delay(50);
  display.ssd1306_command(0x81);
  display.ssd1306_command(0x01);
  display.setTextColor(WHITE);
  delay(50);
  
  draw(msgCounter);

  delay(TIME_UP * 1000);
}

void draw(int msgCount){
  float VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
  display.ssd1306_command(SSD1306_DISPLAYON);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(5,5);
  display.println("LORA SENDER");
  display.setTextSize(1);
  display.setCursor(5,25);
  display.print("Id:");
  display.setCursor(60,25);
  display.print(myPlayerID);
  display.setCursor(5,38);
  display.print("Counter:");
  display.setCursor(60,38);
  display.print(msgCount);
  display.setCursor(100,56);
  display.print(VBAT); 
  display.display();
  delay(3000);
  display.ssd1306_command(SSD1306_DISPLAYOFF);
}

void sendMessage(int msgCount) {
  LoRa.beginPacket();                   // start packet
  String outgoing = "";
  //outgoing += String(myPlayerID) + message + String(msgCount);
  outgoing += String(myPlayerID) + String(msgCount);
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  Serial.print("message sent :");
  Serial.println(outgoing);
  LoRa.end();
  LoRa.sleep();
}
//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//IR
#include <IRrecv.h>
#include <IRsend.h>

#include "./icons.h"
#include "./pins_config.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

IRrecv irrecv(IRreceivePin);
decode_results results;
IRsend irsend(IRtransmitPin);


int myPlayerID             = 14;      // Player ID: 0 - 99
int id_wolf                = myPlayerID;

//stato gioco: 0 recv WERERABBIT, 1 send RABBIT, 2 sparato ex wererabbit, 3, 4 colpito come ex wererabbit, 5 colpito
int stato = 0;
//0 selezionatore di gioco, 1 in gioco
int statoChoose = 1;
//gioco: 0 wererabbit, 1 infected
int choose = 0;

void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(vbatPin, INPUT);

  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(displayPin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(IRtransmitPin, OUTPUT);
  pinMode(IRreceivePin, INPUT);

  digitalWrite(triggerPin, HIGH);
  digitalWrite(displayPin, HIGH);

  irsend.begin();
  irrecv.enableIRIn();

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

  infected_first_frame();

  delay(3000);
  Serial.println("Setup OK!");
}


void loop() {
  
  //gestioneDisplayOnOff();

  if(statoChoose == 0){
    if(chooseState == 0 && digitalRead(displayPin) == LOW){
      chooseState = 1;
      choose++;
      if(choose > 3)
        choose = 0;
      switch(choose){
        case 0:
          Serial.println("selected wererabbit game");
          wererabbit_first_frame();
        break;
        case 1:
          Serial.println("selected infected game");
          infected_first_frame();
        break;
        case 2:
          Serial.println("selected hide flag game");
          flag_first_frame();
        break;
      }
      
    }
    if(triggerState == 0 && digitalRead(triggerPin) == LOW){
      triggerState = 1;
      statoChoose = 1;
    }
    gestioneChoose();
    gestioneTrigger();
  }
  if(statoChoose == 1){
    switch(choose){
      case 0:
        wererabbitGame();
      break;
      case 1:
        infectedGame();
      break;
      case 2:
      break;
    }
  }
}

//gestione display ON OFF
int displayState = 0;
int exDisplayState = 0;
long lastDisplayOnOffTime = 0;

void gestioneDisplayOnOff(){
  if((millis()-lastDisplayOnOffTime)>100){
    lastDisplayOnOffTime = millis();
    
    exDisplayState = displayState;
    displayState = digitalRead(displayPin);
    if(displayState == 0 && exDisplayState != displayState){
      exDisplayState = displayState;
      displayOnOff = displayOnOff * -1 + 1;
      display.ssd1306_command(SSD1306_DISPLAYOFF+displayOnOff);
      schermata_nera();
    }
  }
}

void gestioneTrigger(){
  if((millis()-lastTriggerTime)>100){
    lastTriggerTime = millis();
    if(digitalRead(triggerPin) == HIGH ){
      triggerState = 0;
    }
  }
}

void gestioneChoose(){
  if((millis()-lastChooseTime)>100){
    lastChooseTime = millis();
    if(digitalRead(displayPin) == HIGH ){
      chooseState = 0;
    }
  }
}


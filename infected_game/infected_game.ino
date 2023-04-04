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

#include "./variables.h"
#include "./icons.h"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

IRrecv irrecv(IRreceivePin);
decode_results results;
IRsend irsend(IRtransmitPin);


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

  Serial.println("Setup OK!");
}


void loop() {

  //gestioneDisplayOnOff();
  triggerRelease();

  if(gameTimer == gameTime){
    Serial.println("START GAME!");
    infected_first_frame();
    waitStartGame();
    //quando esce inizio a giocare
    gameTimer -= 1;
  }

  if((millis() - lastGameTime) > 1000){
    lastGameTime = millis();
    gameTimer -= 1;
  }

  if(gameTimer <= 0){
    Serial.println("END GAME!");
    fineGioco();
    waitStartGame();
    gameTimer == gameTime;
  }

  
  //Stato SANO
  if(gameState == 1){
    lora_recv();
    if (millis() - lastSendTime > interval) {
      sendMessage();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      lastSendTime = millis();            // timestamp the message
      interval = random(3000) + 9000;    // 9 - 12 seconds
    }
    if((millis()-lastCleanTime)>1000){
      lastCleanTime = millis();
      if(displayOnOff == 1)
        schermata_send();
    }    
    if(triggerState == 0 && digitalRead(triggerPin) == LOW && ammo > 0){
      shoot();
      triggerState = 1;
      ammo--;
      if(ammo <= 0)
        ammo = 16;
      if(displayOnOff == 1)
        schermata_send();
    }
    //triggerRelease();
  }


  //Stato stato appena INFETTATO
  if(gameState == 2){
    //invio di essermi infettato e aspetto risposta per 3 sec e provo per 2 volte
    int j = 0;
    while(j < 2 && gameState == 2){
      lora_recv();
      if (millis() - lastSendTime > interval) {
        sendMessage();
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        lastSendTime = millis();
        interval = random(1000) + 3000;    // 3 - 4 seconds
        circle_size = 18;
        j++;
      }
      if (millis() - lastCircleTime > 300) {
        schermata_infettato();
        lastCircleTime = millis();
      }
    }
    lastSendTime = millis();
    gameState = 3;
  }

  //Sono INFETTO
  if(gameState == 3){
    //SONO L INFETTO, ascolto ricevitore ir e rssi degli altri
    receiveIR();
    lora_recv();
    if((millis()-lastCleanTime)>1000){
      lastCleanTime = millis();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      if(displayOnOff == 1 && triggerState == 0)
        schermata_recv_draws();
    }
    if((millis()-lastRecvTime_ctl)>36000){
      lastRecvTime_ctl = millis();
      lastCleanTime = millis();
      memset(senders, 0, sizeof(senders));
    }

    if(triggerState == 0 && digitalRead(triggerPin) == LOW){
      if (millis() - lastSendTime > interval) {
        sendMessage();
        lastSendTime = millis();
        interval = random(1000) + 4000;    // 4 - 5 secondi
      }
      triggerState = 1;
    }
    if(triggerState == 1){
      if(displayOnOff == 1)
        schermata_send_infected();
      lastCleanTime = millis();
    }
  }
  

  //sono stato COLPITO
  if(gameState == 4){
    int j = 0;
    //invio per 2 volte che sono stato colpito per far comparire la scritta HIT all altro giocatore.
    while(j < 2 && gameState == 4){
      if (millis() - lastSendTime > interval) {
        sendMessage();
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        lastSendTime = millis();            // timestamp the message
        interval = random(3000) + 9000;    // 9 - 12 seconds
        j++;
      }
      if (millis() - lastCircleTime > 1000) {
        schermata_colpito();
        lastCircleTime = millis();
      }
    }
    gameState = 5;
  }

  //sono MORTO
  if(gameState == 5){
    lora_recv(); // aggiorno valori giocatori
    if (millis() - lastCircleTime > 1000) {
      if(displayOnOff == 1)
        schermata_wait();
      lastCircleTime = millis();
    }
    if (millis() - lastSendTime > interval) {
      sendMessage();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      lastSendTime = millis();            // timestamp the message
      interval = random(3000) + 9000;    // 9 - 12 seconds
    }
  }
    
  //stato di PROVA
  if(gameState == 9){
    VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
    //schermata_HIT();
    //schermata_colpito();
    //schermata_infettato();
    schermata_recv_infected();
    //schermata_wait();
    delay(300);
  }

}

void waitStartGame(){
  Serial.println("Wait trigger!");
  
  delay(100);
  int exit = 0;
  triggerState = 0;
  unsigned long tempo_start = millis();
  while(exit == 0){
    if(triggerState == 0 && digitalRead(triggerPin) == LOW){
      triggerState = 1;
      tempo_start = millis();
    }
    if((millis()-lastTriggerTime)>100){
      lastTriggerTime = millis();
      if(triggerState == 1 && digitalRead(triggerPin) == HIGH){
        if(millis() - tempo_start > 2000)
          exit = 1;
      }
    }
  }
  triggerState = 0;
}

void fineGioco(){
  int infects = 0;
  int rabbits = 0;
  int death = 0;
  for(int i = 0; i<100; i++){
      if(senders[i] == 1){
        infects++;
      }
      else if(senders[i] == 2){
        rabbits++;
      }
      else{
        death++;
      }
    }
  if(gameState == 2){
    schermata_win();
  }
  if(infects > 0){
    if(gameState == 0)
      schermata_lose();
    else
      schermata_win();
  }else if(rabbits > 0){
    if(gameState == 0)
      schermata_win();
    else
      schermata_lose();
  }else{
    schermata_lose();
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

void triggerRelease(){
  if((millis()-lastTriggerTime)>100){
    lastTriggerTime = millis();
    if(digitalRead(triggerPin) == HIGH ){
      triggerState = 0;
    }
  }
}

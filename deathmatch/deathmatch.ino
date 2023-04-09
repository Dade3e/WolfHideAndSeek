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
  digitalWrite(speakerPin, HIGH);

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

  first_frame();

  Serial.println("Setup OK!");
  digitalWrite(speakerPin, LOW);
}


void loop() {

  gestioneDisplayOnOff();
  triggerRelease();

  if(gameTimer == gameTime){
    Serial.println("START GAME!");
    first_frame();
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

  //Sono IN GAME
  if(gameState == 1){
    //SONO IN GAME, ascolto ricevitore ir e rssi degli altri
    receiveIR();
    lora_recv();
    if((millis()-lastCleanTime)>1000){
      lastCleanTime = millis();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      if(displayOnOff == 1 && triggerState == 0)
        schermata_ammo();
    }
    if (millis() - lastSendTime > interval) {
      sendMessage();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      lastSendTime = millis();            // timestamp the message
      interval = random(3000) + minSendTime;    // 60 - 63 seconds
    }

    if(triggerState == 0 && digitalRead(triggerPin) == LOW && ammo > 0){
      shoot();
      triggerState = 1;
      ammo--;
      if(ammo <= 0)
        ammo = 16;
      if(displayOnOff == 1)
        schermata_ammo();
    }
  }

  //Stato APPENA COLPITO
  if(gameState == 2){
    //per_chi = 13;
    //invio di essermi infettato e aspetto risposta per 3 sec e provo per 2 volte
    int j = 0;
    sendMessage();
    schermata_colpito();
    lastSendTime = millis();
    while(j < 2 && gameState == 2){
      lora_recv();
      if (millis() - lastSendTime > interval) {
        sendMessage();
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        lastSendTime = millis();
        interval = random(1000) + 3000;    // 3 - 4 seconds
        j++;
      }
    }
    lastSendTime = millis();
    gameState = 3;            //se metto 1 diventa un tester, 3 normale
    digitalWrite(speakerPin, LOW);
  }

  //sono MORTO
  if(gameState == 3){
    lora_recv(); // aggiorno valori giocatori
    digitalWrite(speakerPin, blink);
    if (millis() - lastCleanTime > 1000) {
      if(displayOnOff == 1)
        schermata_wait();
      lastCleanTime = millis();
      blink = (blink *-1) +1;
    }
    if (millis() - lastSendTime > interval) {
      sendMessage();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      lastSendTime = millis();            // timestamp the message
      interval = random(3000) + minSendTime;    // 9 - 12 seconds
    }
  }

  //stato di PROVA
  if(gameState == 9){
    VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
    //schermata_HIT();
    //schermata_colpito();
    //schermata_infettato();
    schermata_lose();
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
  int list_teams[100];
  int array_counter[100];
  int w = 0;
  int maxValue = 0;
  int array_index_winners[100];

  //controllo tutti contro tutti
  for(int i = 0; i<100; i++){
    if(senders[i] == 1 && senders_team[i] == 99){
      if(senders_kills[i] > maxValue)
        maxValue = senders_kills[i];
    }
  }
  if(maxValue != 0){
    //Cerco indici col valore max, per vedere se ho piu vincitori e salvo gli indici!
    int q = 0;
    for(int i = 1; i < 100; i++){
      if(senders_kills[i] == maxValue){
        array_index_winners[q] = i;     //array con gli indici dei valori piu alti del array_counter
        q++;
      }    
    }
    for(int i = 1; i < q; i++){
      if(myTeamID == list_teams[array_index_winners[i]])  //ma wtf, ce che cazzo
        schermata_win();
      else
        schermata_lose();
    }
  }  
  
  w = 0;
  for(int i = 0; i<100; i++){
    if(senders[i] == 1 ){
      //il giorcatore i e' vivo, cerco se il suo team e' gia nella array teams
      bool unmod = true;
      for (int j=0;j<w; j++){
        if(list_teams[w] == senders_team[i]){
          array_counter[w] += 1;
          unmod = false;
        }
      }
      //se non viene modificato, non ho trovato il team e lo aggiungo
      if(unmod){
        list_teams[w] = senders_team[i];
        array_counter[w] += 1;
        w++;        
      }
    }
  }
  // cerco il valore max
  maxValue = array_counter[0];

  for(int i = 1; i < w; i++)
  {
      if(array_counter[i] > maxValue) {
          maxValue = array_counter[i];
      }
  }
  //Cerco indici col valore max, per vedere se ho piu vincitori e salvo gli indici!
  int array_counter_patta[w];
  int q = 0;
  for(int i = 1; i < w; i++){
    if(array_counter[i] == maxValue){
      array_counter_patta[q] = i;     //array con gli indici dei valori piu alti del array_counter
      q++;
    }    
  }
  for(int i = 1; i < q; i++){
    if(myTeamID == list_teams[array_counter_patta[i]])  //ma wtf, ce che cazzo
      schermata_win();
    else
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

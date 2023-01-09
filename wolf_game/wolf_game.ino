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

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6


//OLED pins
#define OLED_SDA 21  //4
#define OLED_SCL 22 //15
#define OLED_RST -1 //16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);



const uint8_t vbatPin = 35;
float VBAT;

int rssi = -124;
long lastRecvTime = millis();
String message_recv = "";

int senders[99];
int senders_rssi[99];

long lastSendTime = 0;        // last send time
long lastCircleTime = 0;
long lastCleanTime = 0;
long lastTriggerTime = 0;
int interval = 2000;          // interval between sends
long msgCount = 0;
int circle_size=12;

//0 recv LUPO, 1 send PREY, 2 sparato ex lupo, 3, 4 colpito come ex lupo, 5 colpito
int stato = 1;

int myPlayerID             = 14;      // Player ID: 0 - 99
int id_wolf                = myPlayerID;
int myValue                = 1;

int triggerState = 0;
int displayOnOff = 1; // 1 acceso

int msg_counter2 = 0;

int ammo = 16;

//Pin declarations
int triggerPin             = 14;
int displayPin             = 21;
int IRtransmitPin          = 2;
int IRreceivePin           = 13;
int speakerPin             = 15;

IRrecv irrecv(IRreceivePin);
decode_results results;
IRsend irsend(IRtransmitPin);



//wolf64: 64x64
const uint8_t PROGMEM wolf64[] = {
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0x1, 0xf0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xc0, 0x1, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xc0, 0x1, 0xff, 0x0, 0x0, 0x0, 0x0, 0xff,
0xc0, 0x1, 0x9f, 0xc0, 0x0, 0x0, 0x3, 0xfc, 0xc0, 0x1, 0x87, 0xf0, 0x0, 0x0, 0xf, 0xe0, 0xc0, 0x1, 0xc1, 0xfc, 0x0, 0x0, 0x1f, 0x81, 0xc0, 0x1, 0xc0, 0xff, 0x0,
0x0, 0x7e, 0x1, 0x80, 0x0, 0xc0, 0x3f, 0x80, 0x0, 0xfc, 0x1, 0x80, 0x0, 0xc0, 0x1f, 0xc0, 0x41, 0xf8, 0x1, 0x80, 0x0, 0xc0, 0xf, 0xe0, 0xc3, 0xf0, 0x1, 0x80, 0x0,
0xc0, 0x7, 0xf1, 0xc3, 0xe0, 0x1, 0x80, 0x0, 0xc0, 0x3, 0xf3, 0xe7, 0xc0, 0x3, 0x80, 0x0, 0xe0, 0x1, 0xff, 0xff, 0x80, 0x3, 0x80, 0x0, 0x60, 0x0, 0xff, 0xff, 0x80,
0x83, 0x80, 0x0, 0x60, 0x80, 0xff, 0xff, 0x81, 0x83, 0x0, 0x8, 0x60, 0xc1, 0xff, 0xff, 0xe3, 0x83, 0x10, 0xc, 0x60, 0xe7, 0xff, 0xff, 0xff, 0x83, 0x30, 0xe, 0x70,
0xff, 0xff, 0xff, 0xff, 0x83, 0x70, 0xf, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xc7, 0xf0, 0xf, 0xf1, 0xff, 0xcf, 0xf1, 0xff, 0xff, 0xf0, 0x7, 0xff, 0xff, 0xf, 0xf0, 0x3f,
0xff, 0xf0, 0x7, 0xff, 0xfe, 0xf, 0xf0, 0x1f, 0xff, 0xf8, 0x7, 0xff, 0xfc, 0x1f, 0xf8, 0x7, 0xff, 0xfc, 0x7, 0xff, 0xf0, 0x1f, 0xf8, 0x0, 0xf1, 0xfc, 0x7, 0xe3, 0xc0,
0x1f, 0xf8, 0x0, 0x3, 0xfe, 0xf, 0xc0, 0x0, 0x1f, 0xf8, 0x0, 0x7, 0xfe, 0xf, 0xc0, 0x0, 0x1f, 0xf8, 0x0, 0x7, 0xff, 0x1f, 0xe0, 0x0, 0xf, 0xf0, 0x0, 0xf, 0xff, 0x3f,
0xf0, 0x0, 0xf, 0xf0, 0x0, 0x1f, 0xff, 0x7f, 0xf8, 0xf0, 0xf, 0xf0, 0xf, 0x7f, 0xff, 0x7f, 0xff, 0xfc, 0x7, 0xe0, 0x3f, 0xff, 0xff, 0xff, 0xff, 0x86, 0x7, 0xe0, 0x61,
0xff, 0xff, 0xff, 0xfc, 0xc3, 0x7, 0xe0, 0xc3, 0xf, 0xff, 0xff, 0xf8, 0xe1, 0x87, 0xe1, 0x87, 0x7, 0xff, 0xff, 0xf0, 0x70, 0xc7, 0xe3, 0xe, 0x3, 0xfd, 0xdf, 0xe0,
0x38, 0x67, 0xe6, 0x1c, 0x1, 0xfd, 0x9f, 0xe0, 0x1c, 0x77, 0xee, 0x38, 0x1, 0xfc, 0x9f, 0xc0, 0xf, 0xf7, 0xef, 0xf0, 0x0, 0xfc, 0x1f, 0xc0, 0x3, 0xf7, 0xef, 0xc0, 0x0, 0xfc, 0x1f, 0xc0, 0x0, 0xf7, 0xef, 0x0, 0x0,
0xfc, 0x1f, 0xc0, 0x0, 0x37, 0xec, 0x0, 0x1, 0xec, 0x1d, 0xe0, 0x0, 0x7, 0xe0, 0x0, 0x1, 0xec, 0x19, 0xe0, 0x0, 0x7, 0xe0, 0x0, 0xe3, 0xe4, 0x19, 0xf3, 0x80, 0x7, 0xe0, 0x1, 0xf3, 0xe4, 0x11, 0xff, 0xe0, 0x7, 0xe0,
0x3, 0xff, 0xe0, 0x1, 0xff, 0xf0, 0x7, 0xe0, 0x7, 0xff, 0xe0, 0x1, 0xff, 0xf8, 0x7, 0xe0, 0xf, 0xff, 0xe0, 0x1, 0xcf, 0xfc, 0x7, 0xe0, 0x1f, 0xfe, 0xe0, 0x1, 0x8f, 0xfc, 0x7, 0xe0, 0x1f, 0xfc, 0x60, 0x1, 0xf, 0xfe,
0x7, 0xe0, 0x3f, 0xfc, 0x20, 0x0, 0xf, 0xfe, 0x3, 0xc0, 0x3f, 0xdc, 0x0, 0x0, 0xf, 0x7f, 0x3, 0xc0, 0x7f, 0x8c, 0x0, 0x0, 0xe, 0x3f, 0x0, 0x0, 0x7f, 0x4, 0x0, 0x0, 0xc, 0x3f, 0x80, 0x0, 0xfe, 0x4, 0x0, 0x0, 0x8, 
0x1f, 0x80, 0x0, 0xfc, 0x0, 0x0, 0x0, 0x0, 0xf, 0xc0, 0x1, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x7, 0xe0, 0x3, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x3, 0xf0, 0xf, 0xe0, 0x0, 0x0, 0x0, 0x0, 0x1, 0xfc, 0x3f, 0xc0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x7f, 0xff, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xfe, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xf, 0xf8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

//wolf40: 40x40
const uint8_t PROGMEM wolf40[] = {
B00001111, B10000000, B00000000, B00000001, B11100000,
B00001000, B11100000, B00000000, B00000111, B00100000,
B00001000, B00110000, B00000000, B00011100, B00100000,
B00000100, B00011100, B00000000, B00111000, B00100000,
B00000100, B00001110, B00000100, B01110000, B00100000,
B00000100, B00000111, B00001100, B11100000, B00100000,
B00000110, B00000011, B11011101, B11000000, B01000000,
B00000010, B00000001, B11111111, B10000010, B01000000,
B00000010, B00100000, B11111111, B00000110, B01001000,
B00100010, B00110011, B11111111, B10001100, B01011000,
B00110010, B00111111, B11111111, B11111110, B11111000,
B00111011, B01111111, B00111100, B01111111, B11111000,
B00011111, B11111100, B01111110, B00001111, B11111000,
B00111111, B11110000, B01111110, B00000001, B11111100,
B00111100, B11000000, B01111110, B00000000, B00011110,
B01111000, B00000000, B01111110, B00000000, B00011110,
B01111100, B00000000, B01111110, B00000000, B00111111,
B01111110, B11100000, B00111100, B00000111, B01111111,
B01111111, B10011000, B00111100, B00011001, B11111111,
B11111111, B10000100, B00111100, B00100001, B11111111,
B11111110, B11000010, B00111100, B01000011, B01111111,
B11111100, B01110001, B00111100, B10001110, B00111111,
B11111100, B00011001, B10111101, B10011000, B00111101,
B10111000, B00001111, B10111101, B11110000, B00011100,
B10111000, B00000011, B10111101, B11000000, B00011100,
B00111000, B00000000, B00111100, B00000000, B00011100,
B00111100, B00000000, B00111100, B00000000, B00110100,
B00111110, B11000000, B00111100, B00000111, B01110000,
B00101111, B11110000, B00111100, B00001111, B11110000,
B00001111, B11111000, B00111100, B00011111, B11010000,
B00001101, B11111000, B00111100, B00011111, B11000000,
B00001001, B11111100, B00111100, B00111111, B01000000,
B00000001, B01111100, B00011000, B00111110, B00000000,
B00000001, B01111110, B00000000, B01111110, B00000000,
B00000000, B00111110, B00000000, B01111100, B00000000,
B00000000, B00011111, B00000000, B11111000, B00000000,
B00000000, B00001111, B10000001, B11110000, B00000000,
B00000000, B00000111, B11000011, B11100000, B00000000,
B00000000, B00000001, B11111111, B10000000, B00000000,
B00000000, B00000000, B11111111, B00000000, B00000000
};

//skull32: 32x32
const uint8_t PROGMEM skull32[] = {
B00000000, B00000111, B11110000, B00000000, 
B00000000, B00111000, B00001110, B00000000, 
B00000000, B01000000, B00000001, B00000000, 
B00000001, B10000000, B00000000, B11000000, 
B00000010, B00000000, B00000000, B00100000, 
B00000100, B00000000, B00000000, B00010000, 
B00000100, B00000000, B00000000, B00010000, 
B00001000, B00000000, B00000000, B00001000, 
B00010000, B00000000, B00000000, B00000100, 
B00010000, B00000000, B00000000, B00000100, 
B00010000, B00000000, B00000000, B00000100, 
B00100000, B01110000, B00000111, B00000010, 
B00100001, B11111100, B00011111, B11000010, 
B00100001, B11111100, B00011111, B11000010, 
B00100011, B11111110, B00111111, B11100010, 
B00100011, B11111110, B00111111, B11100010, 
B00100011, B11111110, B00111111, B11100010, 
B00100001, B11111100, B00011111, B11000010, 
B00010001, B11111100, B00011111, B11000100, 
B00010000, B01110000, B00000111, B00000100, 
B00010000, B00000000, B00000000, B00000100, 
B00001000, B00000000, B00000000, B00001000, 
B00000100, B00000000, B00000000, B00010000, 
B00000100, B00000000, B00000000, B00010000, 
B00000010, B00000000, B00000000, B00100000, 
B00000001, B10000000, B00000000, B11000000, 
B00000000, B10000000, B00000000, B10000000, 
B00000000, B10001000, B10001000, B10000000, 
B00000000, B10001000, B10001000, B10000000, 
B00000000, B11111111, B11111111, B10000000, 
B00000000, B00000000, B00000000, B00000000, 
B00000000, B00000000, B00000000, B00000000
};

//wolf32: 32x32
const uint8_t PROGMEM wolf32[] = {
B00000111, B00000000, B00000001, B11000000, 
B00000101, B10000000, B00000011, B01000000, 
B00000100, B11000000, B00000110, B01000000, 
B00000100, B11110000, B10001100, B01000000, 
B00000110, B01111001, B10011000, B01000000, 
B00000010, B00111101, B10110000, B11000000, 
B00000010, B00011111, B11110010, B10000000, 
B00010010, B10111111, B11111010, B10010000, 
B00011011, B11110111, B11011110, B10110000, 
B00011111, B11100111, B11000111, B11110000, 
B00011111, B00000111, B11000001, B11111000, 
B00111100, B00000111, B11000000, B11111000, 
B00111100, B00000011, B10000000, B11111100, 
B01111100, B11000011, B10000110, B11111100, 
B01111111, B11100011, B10001101, B01111100, 
B01111100, B00110011, B10011000, B01110100, 
B01101100, B10011011, B10110010, B01110000, 
B01001000, B01001011, B10100100, B01111000, 
B01011100, B00111011, B10111000, B01111000, 
B01011101, B00000011, B10000000, B00111000, 
B00011111, B00000011, B10000001, B10101000, 
B00011111, B10000011, B10000011, B11100000, 
B00010111, B10000011, B10000111, B11100000, 
B00000111, B11000011, B10001111, B11100000, 
B00000011, B11100001, B00001111, B01100000, 
B00000011, B11100000, B00011110, B00100000, 
B00000010, B11111000, B00011110, B00000000, 
B00000000, B00111100, B00111000, B00000000, 
B00000000, B00001111, B11110000, B00000000, 
B00000000, B00000011, B11000000, B00000000,
B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000
};

//Rabbit 16x16
const uint8_t PROGMEM rabbit16[]{
B00001100, B00110000,
B00011110, B01111000, 
B00010010, B01001000,
B00010010, B01001000, 
B00010010, B01001000, 
B00010010, B01001000, 
B00010010, B01001000, 
B00010110, B01101000, 
B00011011, B11011000,
B00110101, B10101100, 
B00111011, B11011100, 
B00111111, B11111100, 
B00111010, B01011100, 
B00011000, B00011000, 
B00001111, B11110000, 
B00000000, B00000000
};

//infinity 16x6
const uint8_t PROGMEM infin[]{
B00011100, B00111000,
B00100010, B01000100,
B01000001, B10000010,
B01000001, B10000010,
B00100010, B01000100,
B00011100, B00111000
};

//infinity 16x5
const uint8_t PROGMEM little_infin[]{
B00011100, B00111000,
B00100010, B01000100,
B00100001, B10000100,
B00100010, B01000100,
B00011100, B00111000
};


//bullet 8X16
const uint8_t PROGMEM bullet8X16[]{
B00011000, 
B00111100, 
B00110100, 
B01110010, 
B01110010, 
B11111111, 
B11111111, 
B11110011, 
B11110011, 
B11110011, 
B11110011, 
B11110011, 
B11110011, 
B11110011, 
B01111110, 
B11111111
};
//bullet 9X20
const uint8_t PROGMEM bullet8X20[]{
0x18, 0x3c, 0x3c, 0x7a, 0x7a, 0x7e, 0xff, 0xfd, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xf9, 0xfd, 0xff, 0x7e, 0xff,
};


void setup() { 
  //initialize Serial Monitor
  Serial.begin(115200);
  pinMode(vbatPin, INPUT);
  //reset OLED display via software

  pinMode(triggerPin, INPUT_PULLUP);
  pinMode(displayPin, INPUT_PULLUP);
  pinMode(speakerPin, OUTPUT);
  pinMode(IRtransmitPin, OUTPUT);
  pinMode(IRreceivePin, INPUT);

  digitalWrite(triggerPin, HIGH);      // Not really needed if your circuit has the correct pull up resistors already but doesn't harm

  irsend.begin();
  irrecv.enableIRIn(); // Start the receiver

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

  //display.ssd1306_command(SSD1306_DISPLAYON);
  display.ssd1306_command(0x81);
  display.ssd1306_command(0x01);
  //display.dim(true);

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, wolf64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(76,10);
  display.print("WOLF");
  display.setCursor(76,30);
  display.print("HIDE and");
  display.setCursor(76,50);
  display.print("SEEK");
  display.display();


//SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  LoRa.setTxPower(2);
  Serial.println("LoRa Initializing OK!");
  delay(3000);
  Serial.println("Setup OK!");
}

void loop() {
  //display.drawLine()
  // see if a packet was received
  
  gestioneDisplayOnOff();

  if(stato == 0){
    //RECV and SHOOT
    attesa_msg();
    if((millis()-lastSendTime)>1000){
      lastSendTime = millis();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      if(displayOnOff == 1 && triggerState == 0)
        schermata_recv_draws();
    }
    if((millis()-lastCleanTime)>60000){
      lastCleanTime = millis();
      memset(senders, 0, sizeof(senders));
    }
    if(triggerState == 0 && digitalRead(triggerPin) == LOW){
      shoot();
      if(displayOnOff == 1)
        schermata_shoot();
      triggerState = 1;
      ammo--;
      if(ammo <= 0)
        ammo = 16;
    }
    gestioneTrigger();
    
  }

  if(stato == 1){
    //SEND
    receiveIR();
    if (millis() - lastSendTime > interval) {
      sendMessage();
      VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
      lastSendTime = millis();            // timestamp the message
      interval = random(3000) + 9000;    // 9 - 12 seconds
      circle_size = 12;
    }
    if (millis() - lastCircleTime > 300) {
        if(displayOnOff == 1)
          schermata_send();
      lastCircleTime = millis();
    }
  }

  //rivecuto che ho colpito un ex lupo  
  if(stato == 2){
    
    if((millis()-lastCleanTime)>3000){
      lastCleanTime = millis();
      stato = 0; // TORNO LUPO
    }
  }

  //colpito 
  if(stato == 3){
    if((millis()-lastCleanTime)>3000){
      lastCleanTime = millis();
      stato = 1; //DIVENTO PREY
    }
  }

  //colpito ma sono ex lupo
  if(stato == 4){
    stato = 1; //RIMANGO PREY
    myValue = 2; //invio segnale che ero ex lupo
    int j = 0;
    while(j < 3){
      if (millis() - lastSendTime > interval) {
        sendMessage();
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        lastSendTime = millis();            // timestamp the message
        interval = random(3000) + 9000;    // 9 - 12 seconds
        circle_size = 12;
        j++;
      }
      if (millis() - lastCircleTime > 300) {
        schermata_send_wolf();
        lastCircleTime = millis();
      }
    }
    myValue = 1;
  }

  //Sono stato colpito
  if(stato == 5){
    stato = 0; //DIVENTO LUPO
    myValue = 3;
    int j = 0;
    int swap = 0;
    while(j < 4){
      if (millis() - lastSendTime > interval) {
        sendMessage();
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        lastSendTime = millis();            // timestamp the message
        interval = random(3000) + 9000;    // 9 - 12 seconds
        circle_size = 12;
        j++;
      }
      if (millis() - lastCircleTime > 1500) {
        schermata_colpito(swap);
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        
        lastCircleTime = millis();
        swap++;
      }
    }
    myValue = 1; 
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



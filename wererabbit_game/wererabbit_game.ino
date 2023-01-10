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
int txPower = 2;
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
int stato = 6;

int myPlayerID             = 14;      // Player ID: 0 - 99
int id_wolf                = myPlayerID;
int myValue                = 1;

int triggerState = 0;
int displayOnOff = 1; // 1 acceso

int msg_counter2 = 0;

int ammo = 16;

int rabbit_draw = 0;

//Pin declarations
int triggerPin             = 14;
int displayPin             = 21;
int IRtransmitPin          = 2;
int IRreceivePin           = 13;
int speakerPin             = 15;

IRrecv irrecv(IRreceivePin);
decode_results results;
IRsend irsend(IRtransmitPin);

const uint8_t PROGMEM infected_rabbit64[] = {
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0xe0, 0x7, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3, 0x80, 0x1, 0xc0, 0x0, 0x0, 
0x0, 0x0, 0x6, 0x0, 0x0, 0x60, 0x0, 0x0, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x38, 0x0, 0x0, 
0x0, 0x0, 0x38, 0x0, 0x0, 0x1c, 0x0, 0x0, 0x0, 0x0, 0x70, 0x0, 0x0, 0xe, 0x0, 0x0, 
0x0, 0x0, 0xe0, 0x0, 0x0, 0x7, 0x0, 0x0, 0x0, 0x1, 0xe0, 0x0, 0x0, 0x7, 0x80, 0x0, 
0x0, 0x1, 0xc0, 0x0, 0x0, 0x3, 0x80, 0x0, 0x0, 0x3, 0xc0, 0x0, 0x0, 0x3, 0xc0, 0x0, 
0x0, 0x3, 0xc0, 0x0, 0x0, 0x3, 0xc0, 0x0, 0x0, 0x3, 0x80, 0x0, 0x0, 0x1, 0xc0, 0x0, 
0x0, 0x7, 0x80, 0x0, 0x0, 0x1, 0xe0, 0x0, 0x0, 0x7, 0x80, 0xf, 0xf0, 0x1, 0xe0, 0x0, 
0x0, 0x7, 0x80, 0x7f, 0xfe, 0x1, 0xe0, 0x0, 0x0, 0x7, 0x81, 0xff, 0xff, 0x81, 0xe0, 0x0, 
0x0, 0x7, 0x83, 0xf0, 0xf, 0xc1, 0xe0, 0x0, 0x0, 0x7, 0xcf, 0x80, 0x1, 0xf3, 0xe0, 0x0, 
0x0, 0x3, 0xce, 0x0, 0x0, 0x73, 0xc0, 0x0, 0x0, 0x3, 0xfc, 0x0, 0x0, 0x3f, 0xc0, 0x0, 
0x0, 0x3, 0xf8, 0x0, 0x0, 0x1f, 0xc0, 0x0, 0x0, 0x1, 0xf0, 0x0, 0x0, 0xf, 0x80, 0x0, 
0x0, 0x1, 0xf8, 0x0, 0x0, 0x1f, 0x80, 0x0, 0x0, 0x7, 0xfc, 0xc, 0x30, 0x3f, 0xe0, 0x0, 
0x0, 0x1f, 0xff, 0x9e, 0x79, 0xff, 0xf8, 0x0, 0x0, 0x7f, 0xff, 0x12, 0x48, 0xff, 0xfe, 0x0, 
0x0, 0xff, 0xff, 0x12, 0x48, 0xff, 0xff, 0x0, 0x1, 0xff, 0xfe, 0x12, 0x48, 0x7f, 0xff, 0x80, 
0x3, 0xff, 0xc6, 0x12, 0x48, 0x63, 0xff, 0xc0, 0x7, 0xe3, 0x82, 0x12, 0x48, 0x41, 0xc7, 0xe0, 
0x7, 0xc3, 0x80, 0x16, 0x68, 0x1, 0xc3, 0xe0, 0xf, 0x3, 0x80, 0x1b, 0xd8, 0x1, 0xc0, 0xf0, 
0xe, 0x1, 0x80, 0x35, 0xac, 0x1, 0x80, 0x70, 0x1e, 0x1, 0x80, 0x3b, 0xdc, 0x1, 0x80, 0x78, 
0x1c, 0x1, 0xc0, 0x3f, 0xfc, 0x3, 0x80, 0x38, 0x38, 0x1, 0xc0, 0x3a, 0x5c, 0x3, 0x80, 0x1c, 
0x38, 0x0, 0xc0, 0x18, 0x18, 0x3, 0x0, 0x1c, 0x38, 0x0, 0xe0, 0xf, 0xf0, 0x7, 0x0, 0x1c, 
0x30, 0x0, 0xe0, 0x0, 0x0, 0x7, 0x0, 0xc, 0x30, 0x0, 0x70, 0x0, 0x0, 0xe, 0x0, 0xc, 
0x30, 0x0, 0x78, 0x0, 0x0, 0x1e, 0x0, 0xc, 0x30, 0x0, 0x3c, 0x7, 0xe0, 0x3c, 0x0, 0xc, 
0x30, 0x0, 0x1e, 0x7, 0xe0, 0x78, 0x0, 0xc, 0x10, 0x0, 0xf, 0x7, 0xe0, 0xf0, 0x0, 0x8, 
0x10, 0x0, 0x7, 0xc7, 0xe3, 0xe0, 0x0, 0x8, 0x10, 0x0, 0x3, 0xff, 0xff, 0xc0, 0x0, 0x8, 
0x0, 0x0, 0x0, 0xff, 0xff, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x3f, 0xfc, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0xf, 0xf0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f, 0xf8, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x3f, 0xfc, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x7f, 0xfe, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x1, 0xfe, 0x7f, 0x80, 0x0, 0x0, 0x0, 0x20, 0x7, 0xf8, 0x1f, 0xe0, 0x4, 0x0, 
0x0, 0x1c, 0x3f, 0xe0, 0x7, 0xfc, 0x38, 0x0, 0x0, 0x7, 0xff, 0x80, 0x1, 0xff, 0xe0, 0x0, 
0x0, 0x1, 0xfe, 0x0, 0x0, 0x7f, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 
0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0
};

const uint8_t PROGMEM rabbit32_01[] = {
B00000000, B00111000, B00001110, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B01101110, B00111011, B00000000, 
B00000000, B01111110, B00111111, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B01110011, B11100111, B00000000, 
B00000000, B11101101, B11011011, B10000000, 
B00000001, B11101101, B11011011, B11000000, 
B00000001, B11110011, B11100111, B11000000, 
B00000001, B11111111, B11111111, B11000000, 
B00000001, B11111111, B11111111, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000000, B11100000, B00000011, B10000000, 
B00000000, B01110000, B00000111, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B00011111, B11111100, B00000000
};

const uint8_t PROGMEM rabbit32_02[] = {
B00000000, B00111000, B00001110, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B01101110, B00111011, B00000000, 
B00000000, B01111110, B00111111, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B01110011, B11100111, B00000000, 
B00000000, B11101101, B11011011, B10000000, 
B00000001, B11101101, B11011011, B11000000, 
B00000001, B11110011, B11100111, B11000000, 
B00000001, B11111111, B11111111, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000000, B11100000, B00000011, B10000000, 
B00000000, B01110000, B00000111, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B00011111, B11111100, B00000000
};

const uint8_t PROGMEM rabbit32_03[] = {
B00000000, B00111000, B00001110, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B01101110, B00111011, B00000000, 
B00000000, B01111110, B00111111, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B01110011, B11100111, B00000000, 
B00000000, B11101101, B11011011, B10000000, 
B00000001, B11101101, B11011011, B11000000, 
B00000001, B11110011, B11100111, B11000000, 
B00000001, B11111111, B11111111, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000000, B11100000, B00000011, B10000000, 
B00000000, B01100000, B00000011, B00000000, 
B00000000, B00110000, B00000110, B00000000, 
B00000000, B00011111, B11111100, B00000000
};

//wererabbit32: 32x32
const uint8_t PROGMEM rabbit32_04[] = {
B00000000, B00111000, B00001110, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B01101110, B00111011, B00000000, 
B00000000, B01111110, B00111111, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B01110011, B11100111, B00000000, 
B00000000, B11101101, B11011011, B10000000, 
B00000001, B11101101, B11011011, B11000000, 
B00000001, B11110011, B11100111, B11000000, 
B00000001, B11111111, B11111111, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000000, B11100010, B00100011, B10000000, 
B00000000, B01100000, B00000011, B00000000, 
B00000000, B00110000, B00000110, B00000000, 
B00000000, B00011111, B11111100, B00000000
};

//wererabbit32: 32x32
const uint8_t PROGMEM rabbit32_05[] = {
B00000000, B00111000, B00001110, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B01111100, B00011111, B00000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B11000110, B00110001, B10000000, 
B00000000, B01101110, B00111011, B00000000, 
B00000000, B01111110, B00111111, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B00111111, B11111110, B00000000, 
B00000000, B01110011, B11100111, B00000000, 
B00000000, B11101001, B11001011, B10000000, 
B00000001, B11101101, B11011011, B11000000, 
B00000001, B11110011, B11100111, B11000000, 
B00000001, B11111111, B11111111, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000001, B11100110, B00110011, B11000000, 
B00000000, B11100010, B00100011, B10000000, 
B00000000, B01100000, B00000011, B00000000, 
B00000000, B00110000, B00000110, B00000000, 
B00000000, B00011111, B11111100, B00000000
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
  display.drawBitmap(0, 0, infected_rabbit64, 64, 64, WHITE);
  display.setTextSize(1);
  display.setCursor(66,8);
  display.print("WERE");
  display.setCursor(90,14);
  display.print("RABBIT");
  display.setCursor(80,36);
  display.print("HIDE &");
  display.setCursor(86,48);
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

  LoRa.setTxPower(txPower);
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
        schermata_send_wererabbit();
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
        schermata_colpito();
        VBAT = (float)(analogRead(vbatPin)) / 4095*2*3.3*1.1;
        
        lastCircleTime = millis();
      }
    }
    myValue = 1; 
  }

  
  if(stato == 6){
    schermata_ex_wererabbit();
    //schermata_HIT();
    //schermata_send_wererabbit();
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



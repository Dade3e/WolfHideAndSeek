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

//Game pins
#define triggerPin 14
#define displayPin 12
#define IRtransmitPin 2
#define IRreceivePin 13
#define speakerPin 15

#define vbatPin 35
float VBAT;

#define txPower 8
#define spreadingFactor 12

#define myPlayerID 11     // Player ID: 1 - 99

// Stato 1: Sano, 2: infettato, 3: infetto, 4: colpito, 5: morto;
int gameState = 1;     //scelta se essere cacciatore o preda

int per_chi    = myPlayerID;

long gameTime = 30 * 60 ;
long gameTimer = gameTime -1; //se metto -1 starta subito

int sogliaRSSI = -80;

String message_recv = "";

int senders[100];
int senders_rssi[100];

unsigned long lastGameTime = 0;
unsigned long lastSendTime = 0;
unsigned long lastCircleTime = 0;
unsigned long lastCleanTime = 0;
unsigned long lastTriggerTime = 0;
unsigned long lastChooseTime = 0;

unsigned long lastRecvTime = 0;
unsigned long lastRecvTime_ctl = 0;
int interval = 2000;          // interval between sends
unsigned long msgCount = 0;
int circle_size=18;

int triggerState = 0;
int displayOnOff = 1; // 1 acceso

int ammo = 16;




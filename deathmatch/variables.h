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

#define myPlayerID 16     // Player ID: 1 - 99
#define myTeamID 99       // 99 tutti contro tutti

#define minSendTime 60000

// Stato 1: In Game; 2: colpito; 3: morto
int gameState = 1;     //scelta se essere cacciatore o preda

int per_chi    = myPlayerID;

long gameTime = 30 * 60 ;
long gameTimer = gameTime; //se metto -1 starta subito

int sogliaRSSI = -80;

String message_recv = "";

unsigned long lastGameTime = 0;
unsigned long lastSendTime = 0;
unsigned long lastCleanTime = 0;
unsigned long lastTriggerTime = 0;
unsigned long lastRecvTime = 0;

int interval = 2000;          // interval between sends

int kills = 0;

int triggerState = 0;
int displayOnOff = 1; // 1 acceso

int ammo = 16;

int senders[100];
int senders_team[100];
int senders_kills[100];

int blink = 0;



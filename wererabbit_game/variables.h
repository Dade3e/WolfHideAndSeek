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



const uint8_t vbatPin = 35;
float VBAT;

int rssi = -124;
int txPower = 8;
int spreadingFactor = 9;


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
int circle_size=12;

int myValue                = 1;

int triggerState = 0;
int displayOnOff = 1; // 1 acceso

int msg_counter2 = 0;

int ammo = 16;

int rabbit_draw = 0;

//Pin declarations
int triggerPin             = 14;
int displayPin             = 12;
int IRtransmitPin          = 2;
int IRreceivePin           = 13;
int speakerPin             = 15;

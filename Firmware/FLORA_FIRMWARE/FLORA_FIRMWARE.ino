#include <FS.h>
#include <ArduinoJson.h>
#include <math.h>

#include "ESP8266TimerInterrupt.h"
#include "SPI.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WiFiUdp.h>
#include <TimeLib.h>
#include <Ticker.h>

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include <TimeLib.h>
#include <Timezone.h>

// Pick a clock version below!
#define CLOCK_VERSION_IV6
//#define CLOCK_VERSION_IV12
//#define CLOCK_VERSION_IV22

#define AP_NAME "FLORA_"
#define FW_NAME "FLORA"
#define FW_VERSION "4.0"
#define CONFIG_TIMEOUT 300000 // 300000 = 5 minutes

// ONLY CHANGE DEFINES BELOW IF YOU KNOW WHAT YOU'RE DOING!
#define NORMAL_MODE 0
#define OTA_MODE 1
#define CONFIG_MODE 2
#define CONFIG_MODE_LOCAL 3
#define CONNECTION_FAIL 4
#define UPDATE_SUCCESS 1
#define UPDATE_FAIL 2
#define DATA 13
#define CLOCK 14
#define LATCH 15
#define TIMER_INTERVAL_uS 200 // 200 = safe value for 6 digits. You can go down to 150 for 4-digit one. Going too low will cause crashes.

// User global vars
const char* update_path = "/update";
const char* update_username = "flora";
const char* update_password = "flora";
const char* ntpServerName = "pool.ntp.org";

const uint8_t PixelCount = 12; // Addressable LED count
RgbColor colorConfigMode = RgbColor(60, 0, 60);
RgbColor colorConfigSave = RgbColor(0, 0, 60);
RgbColor colorWifiConnecting = RgbColor(90, 45, 0);
RgbColor colorWifiSuccess = RgbColor(0, 60, 0);
RgbColor colorWifiFail = RgbColor(90, 0, 0);
RgbColor colorStartupDisplay = RgbColor(0, 70, 50);
RgbColor red[] = {
  RgbColor(40, 0, 0), // LOW
  RgbColor(60, 0, 0), // MEDIUM
  RgbColor(90, 0, 0), // HIGH
};
RgbColor green[] = {
  RgbColor(0, 30, 0), // LOW
  RgbColor(0, 40, 0), // MEDIUM
  RgbColor(0, 60, 0), // HIGH
};

RgbColor colonColorDefault[] = {
  RgbColor(9, 28, 10), // LOW
  RgbColor(16, 55, 20), // MEDIUM
  RgbColor(20, 85, 30), // HIGH
};

/*
  RgbColor colonColorDefault[] = {
  RgbColor(30, 6, 1), // LOW
  RgbColor(38, 8, 2), // MEDIUM
  RgbColor(50, 10, 2), // HIGH
  };
*/
RgbColor currentColor = RgbColor(0, 0, 0);
//RgbColor colonColorDefault = RgbColor(90, 27, 7);
//RgbColor colonColorDefault = RgbColor(38, 12, 2);

#if !defined(CLOCK_VERSION_IV6) && !defined(CLOCK_VERSION_IV12) && !defined(CLOCK_VERSION_IV22)
#error "You have to pick a clock version! Line 25"
#endif

#if defined(CLOCK_VERSION_IV6)
const uint8_t registersCount = 6;
const uint8_t segmentCount = 8; 
const uint8_t digitPins[registersCount][segmentCount] = {
  {40, 41, 42, 43, 44, 45, 46, 47}, // BR | B | BL | TL | M | T | TR | DOT
  {32, 33, 34, 35, 36, 37, 38, 39}, // BR | B | BL | TL | M | T | TR | DOT
  {27, 25, 26, 28, 29, 30, 31, 24}, // BR | B | BL | TL | M | T | TR | DOT
  {16, 17, 18, 19, 20, 21, 22, 23}, // BR | B | BL | TL | M | T | TR | DOT
  {8, 9, 10, 11, 12, 13, 14, 15}, // BR | B | BL | TL | M | T | TR | DOT
  {0, 1, 2, 3, 4, 5, 6, 7}, // BR | B | BL | TL | M | T | TR | DOT
};
#elif defined(CLOCK_VERSION_IV12)
const uint8_t registersCount = 6;
const uint8_t segmentCount = 7; // IV12 doesn't have dot
const uint8_t digitPins[registersCount][segmentCount] = {
  {46, 47, 41, 40, 42, 44, 45}, // BR | B | BL | TL | M | T | TR | DOT
  {39, 32, 34, 33, 35, 37, 38}, // BR | B | BL | TL | M | T | TR | DOT 
  {31, 24, 26, 25, 27, 29, 30}, // BR | B | BL | TL | M | T | TR | DOT 
  {23, 16, 18, 17, 19, 21, 22}, // BR | B | BL | TL | M | T | TR | DOT 
  {15, 8, 10, 9, 11, 13, 14}, // BR | B | BL | TL | M | T | TR | DOT 
  {1, 0, 4, 3, 5, 7, 2}, // BR | B | BL | TL | M | T | TR | DOT 
};
#elif defined(CLOCK_VERSION_IV22)
const uint8_t registersCount = 4;
const uint8_t segmentCount = 8;
const uint8_t digitPins[registersCount][segmentCount] = {
  {26, 24, 31, 30, 27, 29, 28, 25}, // BR | B | BL | TL | M | T | TR | DOT
  {20, 16, 23, 22, 19, 17, 18, 21}, // BR | B | BL | TL | M | T | TR | DOT
  {14, 9, 10, 8, 13, 11, 12, 15}, // BR | B | BL | TL | M | T | TR | DOT
  {2, 0, 7, 6, 3, 5, 4, 1}, // BR | B | BL | TL | M | T | TR | DOT
};
#endif

uint8_t letter_p[8] = {0, 0, 1, 1, 1, 1, 1, 0};
uint8_t letter_i[8] = {0, 0, 1, 1, 0, 0, 0, 0};
uint8_t dot[8] = {0, 0, 0, 0, 0, 0, 0, 1};
uint8_t numbers[10][8] = {
  {1, 1, 1, 1, 0, 1, 1, 0}, // 0 ==>  BR | B | BL | TL | M | T | TR | DOT
  {1, 0, 0, 0, 0, 0, 1, 0}, // 1 ==>  BR | B | BL | TL | M | T | TR | DOT
  {0, 1, 1, 0, 1, 1, 1, 0}, // 2 ==>  BR | B | BL | TL | M | T | TR | DOT
  {1, 1, 0, 0, 1, 1, 1, 0}, // 3 ==>  BR | B | BL | TL | M | T | TR | DOT
  {1, 0, 0, 1, 1, 0, 1, 0}, // 4 ==>  BR | B | BL | TL | M | T | TR | DOT
  {1, 1, 0, 1, 1, 1, 0, 0}, // 5 ==>  BR | B | BL | TL | M | T | TR | DOT
  {1, 1, 1, 1, 1, 1, 0, 0}, // 6 ==>  BR | B | BL | TL | M | T | TR | DOT
  {1, 0, 0, 0, 0, 1, 1, 0}, // 7 ==>  BR | B | BL | TL | M | T | TR | DOT
  {1, 1, 1, 1, 1, 1, 1, 0}, // 8 ==>  BR | B | BL | TL | M | T | TR | DOT
  {1, 1, 0, 1, 1, 1, 1, 0}, // 9 ==>  BR | B | BL | TL | M | T | TR | DOT
};
volatile uint8_t segmentBrightness[registersCount][8];
volatile uint8_t targetBrightness[registersCount][8];

// 32 steps of brightness * 200uS => 6.4ms for full refresh => 160Hz... pretty good!
volatile uint8_t shiftedDutyState[registersCount];
const uint8_t pwmResolution = 32; // should be in the multiples of dimmingSteps to enable smooth crossfade
const uint8_t dimmingSteps = 8;
const uint8_t bri_vals[3] = { // These need to be multiples of 8 to enable crossfade! HIGH must be less or equal as pwmResolution.
  8, // LOW
  16, // MEDIUM
  32, // HIGH
};
// Better left alone global vars
unsigned long configStartMillis, prevDisplayMillis;
uint8_t deviceMode = NORMAL_MODE;
bool timeUpdateFirst = true;
volatile bool toggleSeconds;
byte mac[6];
volatile int dutyState = 0;
volatile uint8_t digitsCache[] = {0, 0, 0, 0};
volatile byte bytes[registersCount];
volatile byte prevBytes[registersCount];
volatile uint8_t bri = 0;
volatile uint8_t crossFadeTime = 0;
uint8_t timeUpdateStatus = 0; // 0 = no update, 1 = update success, 2 = update fail,
uint8_t failedAttempts = 0;
RgbColor colonColor;
IPAddress ip_addr;

TimeChangeRule EDT = {"EDT", Last, Sun, Mar, 1, 120};  //UTC + 2 hours
TimeChangeRule EST = {"EST", Last, Sun, Oct, 1, 60};  //UTC + 1 hours
Timezone TZ(EDT, EST);
NeoPixelBus<NeoGrbFeature, NeoWs2813Method> strip(PixelCount);
NeoPixelAnimator animations(PixelCount);
DynamicJsonDocument json(2048); // config buffer
//Ticker pwm_ticker;
Ticker fade_animation_ticker;
Ticker onceTicker;
Ticker colonTicker;
ESP8266Timer ITimer;
ESP8266WebServer server(80);
WiFiUDP Udp;
ESP8266HTTPUpdateServer httpUpdateServer;
unsigned int localPort = 8888;  // local port to listen for UDP packets


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(115200);

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS: Failed to mount file system");
  }
  readConfig();

  initStrip();
  initScreen();

  WiFi.macAddress(mac);

  const char* ssid = json["ssid"].as<const char*>();
  const char* pass = json["pass"].as<const char*>();
  const char* ip = json["ip"].as<const char*>();
  const char* gw = json["gw"].as<const char*>();
  const char* sn = json["sn"].as<const char*>();

  if (ssid != NULL && pass != NULL && ssid[0] != '\0' && pass[0] != '\0') {
    Serial.println("WIFI: Setting up wifi");
    WiFi.mode(WIFI_STA);

    if (ip != NULL && gw != NULL && sn != NULL && ip[0] != '\0' && gw[0] != '\0' && sn[0] != '\0') {
      IPAddress ip_address, gateway_ip, subnet_mask;
      if (!ip_address.fromString(ip) || !gateway_ip.fromString(gw) || !subnet_mask.fromString(sn)) {
        Serial.println("Error setting up static IP, using auto IP instead. Check your configuration.");
      } else {
        WiFi.config(ip_address, gateway_ip, subnet_mask);
      }
    }

    // serializeJson(json, Serial);
    strip.ClearTo(colorWifiConnecting);
    strip_show();

    WiFi.begin(ssid, pass);

    //startBlinking(200, colorWifiConnecting);


    for (int i = 0; i < 1000; i++) {
      if (WiFi.status() != WL_CONNECTED) {
        if (i > 200) { // 20s timeout
          deviceMode = CONFIG_MODE;
          //colonTicker.detach();
          strip.ClearTo(colorWifiFail);
          strip_show();
          Serial.print("WIFI: Failed to connect to: ");
          Serial.print(ssid);
          Serial.println(", going into config mode.");
          delay(500);
          break;
        }
        blankAllDigits();
        setDot(i % registersCount, 1);
        delay(100);
      } else {
        //colonTicker.detach();
        strip.ClearTo(colorWifiSuccess);
        strip_show();
        Serial.println("WIFI: Connected...");
        Serial.print("WIFI: Connected to: ");
        Serial.println(WiFi.SSID());
        Serial.print("WIFI: Mac address: ");
        Serial.println(WiFi.macAddress());
        Serial.print("WIFI: IP address: ");
        Serial.println(WiFi.localIP());
        delay(1000);
        break;
      }
    }

  } else {
    deviceMode = CONFIG_MODE;
    Serial.println("SETTINGS: No credentials set, going to config mode.");
  }

  if (deviceMode == CONFIG_MODE || deviceMode == CONNECTION_FAIL) {
    startConfigPortal(); // Blocking loop
  } else {
    ndp_setup();
    startLocalConfigPortal();
  }

  colonColor = colonColorDefault[bri];
  initScreen();

  //initColon();

  if (json["rst_cycle"].as<unsigned int>() == 1) {
    cycleDigits();
    delay(500);
  }

  if (json["rst_ip"].as<unsigned int>() == 1) {
    showIP(5000);
    delay(500);
  }

}

// the loop function runs over and over again forever
void loop() {

  if (timeUpdateFirst == true && timeUpdateStatus == UPDATE_FAIL || deviceMode == CONNECTION_FAIL) {
    setAllDigitsTo(0);
    strip.ClearTo(red[2]); // red
    strip_show();
    delay(10);
    return;
  }

  if (millis() - prevDisplayMillis >= 1000) { //update the display only if time has changed
    prevDisplayMillis = millis();
    toggleNightMode();
    /*
        if (
          (json["cathode"].as<int>() == 1 && (hour() >= 2 && hour() <= 6) && minute() < 10) ||
          (json["cathode"].as<int>() == 2 && (((hour() >= 2 && hour() <= 6) && minute() < 10) || minute() < 1))
        ) {
          //healingCycle(); // do healing loop if the time is right :)
        } else {
    */
    if (timeUpdateStatus) {
      if (timeUpdateStatus == UPDATE_SUCCESS) {
        setTemporaryColonColor(5, green[bri]);
      }
      if (timeUpdateStatus == UPDATE_FAIL) {
        if (failedAttempts > 2) {
          colonColor = red[bri];
        } else {
          setTemporaryColonColor(5, red[bri]);
        }
      }
      timeUpdateStatus = 0;
    }

    handleColon();
    showTime();
    /*
        }
    */
  }

  animations.UpdateAnimations();
  strip_show();

  server.handleClient();
  //MDNS.update();
  delay(10); // Keeps the ESP cold!

}

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>


#define CLK_VOL 18
#define DT_VOL  17
#define CLK_RAD 19
#define DT_RAD  23

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define OLED_ADDR     0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


StaticJsonDocument<512> doc;
String jsonBuffer;


int counter_vol = 50;
int counter_rad = 0;

int lastCLK_VOL;
int lastCLK_RAD;

int last_sent_vol = -1;

unsigned long lastDraw = 0;
const unsigned long DRAW_INTERVAL = 200;


void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true) {}
  }

  pinMode(CLK_VOL, INPUT_PULLUP);
  pinMode(DT_VOL,  INPUT_PULLUP);
  pinMode(CLK_RAD, INPUT_PULLUP);
  pinMode(DT_RAD,  INPUT_PULLUP);

  lastCLK_VOL = digitalRead(CLK_VOL);
  lastCLK_RAD = digitalRead(CLK_RAD);

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Waiting for data...");
  display.display();
}


void updateEncoders() {
  int clk_vol;
  int clk_rad;
  bool volChanged = false;

  // ---- VOLUME ----
  clk_vol = digitalRead(CLK_VOL);
  if (clk_vol != lastCLK_VOL) {
    if (digitalRead(DT_VOL) != clk_vol) counter_vol++;
    else counter_vol--;
    counter_vol = constrain(counter_vol, 1, 99);
    volChanged = true;
  }
  lastCLK_VOL = clk_vol;

  // ---- RADIO ----
  // ---- RADIO ----
  clk_rad = digitalRead(CLK_RAD);
  if (clk_rad != lastCLK_RAD) {
    counter_rad++;
  }

  if (counter_rad != 0) {
    Serial.print("Switch:");
    Serial.println(counter_rad);
    counter_rad = 0;
  }

  lastCLK_RAD = clk_rad;


  // ---- SEND VOLUME TO PI ----
  if (volChanged && counter_vol != last_sent_vol) {
    Serial.print("VOL:");
    Serial.println(counter_vol);
    last_sent_vol = counter_vol;
  }
}


void drawDashboard() {
  display.clearDisplay();
  display.setCursor(0, 0);

  float cpu  = doc["CPU Usage %"] | 0.0;
  float ram  = doc["RAM usage %"] | 0.0;
  float disk = doc["Used disk %"] | 0.0;
  const char* ip   = doc["IP"] | "-";
  const char* dt   = doc["Date & Time"] | "-";
  const char* ctry = doc["Country"] | "-";

  display.print("CPU "); display.print(cpu, 1); display.println("%");
  display.print("RAM "); display.print(ram, 1); display.println("%");
  display.print("DSK "); display.print(disk, 1); display.println("%");

  display.print("@ "); display.println(ip);
  display.print("# "); display.println(dt);
  display.print("[] "); display.println(ctry);

  display.println();
  display.print("VOL "); display.println(counter_vol);
  display.print("RAD "); display.println(counter_rad);

  display.display();
}


void handleSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      DeserializationError err = deserializeJson(doc, jsonBuffer);
      jsonBuffer = "";

      if (!err) {
        unsigned long now = millis();
        if (now - lastDraw > DRAW_INTERVAL) {
          drawDashboard();
          lastDraw = now;
        }
      }
    } else {
      jsonBuffer += c;
    }
  }
}


void loop() {
  updateEncoders();
  handleSerial();
}

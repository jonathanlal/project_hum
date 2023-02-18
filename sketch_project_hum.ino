#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <AbleButtons.h>
#include "NewPing.h"
#include "DHT.h"
#include "Melody.h"
#include "Musician.h"
#include <neotimer.h>

using Button = AblePullupClickerButton;
#define BUTTON_PIN D5
Button btn(BUTTON_PIN); // The button to check.

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define DHTPIN 12     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);
float temp;
float humidity;

#define PIN_BUZZER 2 // GPIO 2 (d4)
Musician musician(PIN_BUZZER);
Melody mozartNachtmusik("g<<r-d- | g<< r-d-(g<dg<b)-d<*r | c*<<r-a-c*<<r-a- |(c*<af#<a)-d<r | (gr)- g. (bag | (gag)/3:1 f#)- f#. (ac*f# | ag)- g.  (bag | (gag)/3:1 f#)- f#. (ac*f#)- | ((grgr)-- (gf#ef#)--)>> ((grgr)-- (baga)--)> | (brbr)-- (d*c*bc*)-- d*< r | ((de)+  | (d-c.)-c (c-b_.)-  b_ | (( b-a.)- a (gf#ef# | (grarbr)>)- r )_)> ", 140);

// Maximum distance we want to ping for (in centimeters).
#define MAX_DISTANCE 400	

#define TRIG D8 //D8
#define ECHO D7 //D7
float distance;
float remaining_distance;

#define LED D3 //D4

// NewPing setup of pins and maximum distance.
NewPing sonar(TRIG, ECHO, MAX_DISTANCE);
int sonar_iterations = 5;

float initial_distance;
float evaporationRate; //initial
String evaporationUnits = " cm/h"; 
int timerAmount = 3600000; //1hour in milliseconds
float hoursRemaining;

float containerHeight = 14.6; // double check value with no water
int DANGER_WATER_LEVEL = 12.6;
int selectedScreen = 1;

Neotimer mytimer = Neotimer(timerAmount); // Set timer's preset to 1hour
int count = 0;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  delay(1500);
  display.setRotation(2);

  btn.begin(); // Always begin() each button to initialise it.
  dht.begin();

  musician.setBreath(1); //milliseconds of silence at the end of notes
  musician.setLoudnessLimit(0, 16); //Depending on your hardware

  calculateDistance();
   pinMode(LED, OUTPUT);
}
void loop() {
  calculateDistance();
  onEveryHour();
  onButtonPress();
  onLevelLow();
}

void onEveryHour(){
   if(mytimer.repeat()){
      evaporationRate = initial_distance - remaining_distance;
      hoursRemaining = remaining_distance / evaporationRate;
      
      if(hoursRemaining > 1000 || hoursRemaining < 0){
        hoursRemaining = 0;
      }
      if(evaporationRate < 0){
        evaporationRate = 0;
      }
      
      initial_distance = distance;
      }
}
void calculateDistance(){
  humidity = dht.readHumidity();
  temp = dht.computeHeatIndex(dht.readTemperature(), humidity, false);

  float speed_of_sound = 331.3 + (0.606 * temp);
  long duration = sonar.ping_median()/2;

  distance = duration*(speed_of_sound/10000);
  remaining_distance = containerHeight-distance;

  if(initial_distance == 0){
    initial_distance = remaining_distance;
  }
}
void setDisplay(){
    if(selectedScreen == 1){
          display.ssd1306_command(SSD1306_DISPLAYON);
      mainDisplay();
    }else if(selectedScreen == 2){
          display.ssd1306_command(SSD1306_DISPLAYON);
      distanceDisplay();
    }else if(selectedScreen == 3){
      display.ssd1306_command(SSD1306_DISPLAYOFF);
    }
}
void onButtonPress(){
  btn.handle(); // Always handle() each button in a loop to use it.
  if(btn.resetClicked()){
    setDisplay();
    selectedScreen = selectedScreen+1;
    if(selectedScreen > 3){
      selectedScreen = 1;
    }
  }

}
void mainDisplay() {
    const uint8_t bitmap11[] = {0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x01, 0xe0, 0x00, 0x03, 0xf0, 0x00, 0x07, 0x38, 0x00, 0x0e, 0x1c, 0x00, 0x1c, 0x0e, 0x00, 0x18, 0x06, 0x00, 0x38, 0x07, 0x00, 0x30, 0x03, 0x00, 0x30, 0x03, 0x00, 0x30, 0x03, 0x00, 0x38, 0x07, 0x00, 0x18, 0x06, 0x00, 0x1c, 0x0e, 0x00, 0x0f, 0xfc, 0x00, 0x07, 0xf8, 0x00, 0x00, 0xc0, 0x00};
    const uint8_t bitmap15[] = {0x03, 0xe0, 0x00, 0x03, 0xe0, 0x00, 0x03, 0x70, 0x00, 0x03, 0x70, 0x00, 0x03, 0x70, 0x00, 0x03, 0x70, 0x00, 0x03, 0x70, 0x00, 0x03, 0x70, 0x00, 0x03, 0x70, 0x00, 0x03, 0x70, 0x00, 0x03, 0x70, 0x00, 0x07, 0x70, 0x00, 0x0e, 0x38, 0x00, 0x0c, 0x18, 0x00, 0x0c, 0x18, 0x00, 0x0e, 0x38, 0x00, 0x07, 0xf0, 0x00, 0x03, 0xe0, 0x00};
    const uint8_t bitmap23[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x38, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x07, 0x00, 0x00, 0x03, 0x80, 0x00, 0x07, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x38, 0x00, 0x00, 0x30, 0xff, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.drawBitmap(0, 0, bitmap11, 18, 18, 1);
    display.setCursor(25, 3);
    display.print(humidity);
    display.print("%");
    display.drawLine(0, 21, 128, 21, 1);
    display.drawBitmap(0, 23, bitmap15, 18, 18, 1);
    display.setCursor(25, 26);
    display.print(temp);
    display.cp437(true);
    display.write(248);
    display.print("C");
    display.drawLine(0, 43, 128, 43, 1);
    display.drawBitmap(0, 46, bitmap23, 18, 18, 1);
    display.setCursor(25, 48);
    display.print(remaining_distance);
    display.print("cm");
    display.drawLine(20, 0, 20, 64, 1);
    display.display();
}
void distanceDisplay(){
    const uint8_t bitmap23[] = {0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x03, 0xff, 0xc0, 0x07, 0xc3, 0xe0, 0x0e, 0x00, 0x70, 0x1c, 0x18, 0x38, 0x38, 0x18, 0x1c, 0x30, 0x18, 0x0c, 0x70, 0x18, 0x0e, 0x70, 0x18, 0x0e, 0x60, 0x18, 0x06, 0x60, 0x1c, 0x06, 0x60, 0x1f, 0x06, 0x60, 0x0f, 0x86, 0x70, 0x03, 0x8e, 0x70, 0x00, 0x0e, 0x30, 0x00, 0x0c, 0x38, 0x00, 0x1c, 0x1c, 0x00, 0x38, 0x0e, 0x00, 0x70, 0x07, 0xc3, 0xe0, 0x03, 0xff, 0xc0, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00};
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.setTextWrap(0);
    display.setCursor(25, 0);
    display.println("REMAINING CM:");
    display.setTextSize(3);
    display.setCursor(0, 10);
    display.setTextWrap(0);
    display.setCursor(19, 10);
    display.println(remaining_distance);
    display.setCursor(0, 33);
    display.setTextSize(1);
    display.setCursor(28, 37);
    display.drawLine(0, 33, 128, 33, 1);
    display.drawBitmap(0, 38, bitmap23, 24, 24, 1);
    display.setTextSize(2);
    display.print(hoursRemaining, 1);
    display.print(" hrs");
    display.setCursor(28, 55);
    display.setTextSize(1);
    display.print(evaporationRate);
    display.print(evaporationUnits);
    display.display();
}
void dangerDisplay(){
    const uint8_t bitmap8[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xe7, 0x00, 0x00, 0xe7, 0x00, 0x01, 0xc3, 0x80, 0x01, 0xc3, 0x80, 0x03, 0x99, 0xc0, 0x07, 0x18, 0xe0, 0x07, 0x18, 0xe0, 0x0e, 0x18, 0x70, 0x0e, 0x18, 0x70, 0x1c, 0x18, 0x38, 0x38, 0x00, 0x1c, 0x38, 0x00, 0x1c, 0x70, 0x18, 0x0e, 0x70, 0x18, 0x0e, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x07, 0x7f, 0xff, 0xfe, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    const uint8_t bitmap9[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3c, 0x00, 0x00, 0x7e, 0x00, 0x00, 0xe7, 0x00, 0x00, 0xe7, 0x00, 0x01, 0xc3, 0x80, 0x01, 0xc3, 0x80, 0x03, 0x99, 0xc0, 0x07, 0x18, 0xe0, 0x07, 0x18, 0xe0, 0x0e, 0x18, 0x70, 0x0e, 0x18, 0x70, 0x1c, 0x18, 0x38, 0x38, 0x00, 0x1c, 0x38, 0x00, 0x1c, 0x70, 0x18, 0x0e, 0x70, 0x18, 0x0e, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x07, 0x7f, 0xff, 0xfe, 0x3f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(2);
    display.setCursor(0, 11);
    display.setTextWrap(0);
    display.setCursor(34, 11);
    display.println("FUCK!");
    display.setCursor(0, 29);
    display.setTextWrap(0);
    display.setCursor(4, 29);
    display.println("LEVEL LOW!");
    display.drawBitmap(2, 0, bitmap8, 24, 24, 1);
    display.drawBitmap(102, 0, bitmap9, 24, 24, 1);
    display.setCursor(0, 47);
    display.setTextWrap(0);
    display.setCursor(46, 47);
    display.println(remaining_distance);
    display.print("cm");
    display.display();
}
void playSound(){
    musician.refresh();
  if (!musician.isPlaying()){
       if(distance>DANGER_WATER_LEVEL){
        musician.setMelody(&mozartNachtmusik);
        musician.play();
       }
  }else{
    if(distance<DANGER_WATER_LEVEL){
      musician.stop();
    }
  }
}
void onLevelLow(){
    playSound();
    if(distance>DANGER_WATER_LEVEL){
      dangerDisplay();
       digitalWrite(LED, HIGH);
    }else{
      setDisplay();
       digitalWrite(LED, LOW);
    }
}


/**
  NanoTac example
  Controle Tv LG
*/

#include <Wire.h>
#include <FaBo3Axis_ADXL345.h>

FaBo3Axis fabo3axis;
#include <IRremote.h>
IRsend irsend;

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for SSD1306 display connected using software SPI (default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

int action = 0;
long unsigned int irCode;
String screen = "BEM VINDO";

void setup()
{
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();
  
  Serial.println("Checking I2C device...");
  
  if(fabo3axis.searchDevice()){
    Serial.println("I am ADXL345");
  }
  Serial.println("Init...");
  fabo3axis.configuration();
  fabo3axis.powerOn();

  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(7, INPUT);

  display.setTextSize(2);             
  display.setTextColor(WHITE);        
  display.setCursor(0,0);             
  display.println(screen);
  display.display();
  delay(2000);
}



void loop() {
  int x;
  int y;
  int z;
  
  fabo3axis.readXYZ(&x,&y,&z);

//  Serial.print("x: ");
//  Serial.print(x);
//  Serial.print(", y: ");
//  Serial.print(y);
//  Serial.print(", z: ");
//  Serial.println(z);

  display.clearDisplay();
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(8, LOW);

  int ldr = analogRead(A7);
  display.setTextSize(2);            
  display.setTextColor(WHITE);               
  

  display.setTextSize(1);            
  display.setTextColor(WHITE);        
  display.setCursor(0,20);           
  display.print("x: ");
  display.println(x);
  display.print("y: ");
  display.println(y);
  display.print("z: ");
  display.println(z);
  display.print("luz: ");
  display.println(ldr);

  display.setTextSize(2);             
  display.setTextColor(WHITE);        
  display.setCursor(0,0);             
  display.println(screen);

  // action = 1 - CH +
  // action = 2 - CH -
  // action = 3 - VOL +
  // action = 4 - VOL -
  // action = 5 - POWER

  if (x > 10) {
    action = 3;
  } else if (x < -10) {
    action = 4;
  }else if(y > 10) {
    action = 2;
  } else if(y < -10) {
    action = 1;
  } else if(z < 0) {
    action = 5;
  } else {
    action = 0;
  }
  
  switch (action) {
    case 1: 
      irCode=0x20DF00FF;
      screen = "CH +";
      break;
    case 2: 
      irCode=0x20DF807F;
      screen = "CH -";
      break;
    case 3: 
      irCode=0x20DF40BF;
      screen = "VOL +";
      break;
    case 4: 
      irCode=0x20DFC03F;
      screen = "VOL -";
      break;
    case 5: 
      irCode=0x20DF10EF;
      screen = "POWER";
      break;
    default:
      irCode=0x0;
      screen = "LG";
    break;
      
  }


  if( !digitalRead(7) ) {
    irsend.sendNEC(irCode,32);
    digitalWrite(5, HIGH);
    delay(50);
    digitalWrite(5, LOW);
    delay(100);
  }

  display.display();
  delay(50);
}

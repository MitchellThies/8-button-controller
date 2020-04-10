#include <FastLED.h>
#define NUM_LEDS 50
#define DATA_PIN 2
#define pushButtonPin 3
#define potPin A5
#define maxNumberPatterns 6
#define BRIGHTNESS  65//128

CRGB leds[NUM_LEDS];
bool prevButtonState = 0;
bool currentButtonState = 1;
int whichPattern = 0;

int potValue = 0;
int minDelay = 10;
int maxDelay = 100;

//Temperature information
#define TEMPERATURE_1 Tungsten100W
#define TEMPERATURE_2 OvercastSky

//Palette inforation
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

/*
  by Ody
  12-2015
*/

/*********BACK*********
   49 36 35 22 21 08 07
   48 37 34 23 20 09 06
   47 38 33 24 19 10 05
   46 39 32 25 18 11 04
   45 40 31 26 17 12 03
   44 41 30 27 16 13 02
   43 42 29 28 15 14 01
*********FRONT*********/

int matrix[7][7] = {
  {43, 44, 45, 46, 47, 48, 49},
  {42, 41, 40, 39, 38, 37, 36},
  {29, 30, 31, 32, 33, 34, 35},
  {28, 27, 26, 25, 24, 23, 22},
  {15, 16, 17, 18, 19, 20, 21},
  {14, 13, 12, 11, 10, 9, 8},
  {1, 2, 3, 4, 5, 6, 7}
};

int colpin1 = 7;
int colpin2 = 8;
int colpin3 = 9;
int colpin4 = 10;
int colpin5 = 11;
int colpin6 = 12;
int colpin7 = 13;


int rowpin1 = 6;
int rowpin2 = A2;
int rowpin3 = A3;
int rowpin4 = A4;
int rowpin5 = A5;

int strobe = 4; // strobe pins on digital 4
int res = 5; // reset pins on digital 5
int left[7]; // store band values in these arrays
int right[7];
int band;
void setup()
{

  Serial.begin(115200);
  pinMode(res, OUTPUT); // reset
  pinMode(strobe, OUTPUT); // strobe

//  pinMode(colpin1, OUTPUT);
//  pinMode(colpin2, OUTPUT);
//  pinMode(colpin3, OUTPUT);
//  pinMode(colpin4, OUTPUT);
//  pinMode(colpin5, OUTPUT);
//  pinMode(colpin6, OUTPUT);
//  pinMode(colpin7, OUTPUT);
//
//  pinMode(rowpin1, OUTPUT);
//  pinMode(rowpin2, OUTPUT);
//  pinMode(rowpin3, OUTPUT);
//  pinMode(rowpin4, OUTPUT);
//  pinMode(rowpin5, OUTPUT);

  digitalWrite(res, LOW); // reset low
  digitalWrite(strobe, HIGH); //pin 5 is RESET on the shield

  delay(2000);
  FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;

  pinMode(pushButtonPin, INPUT_PULLUP);
  //  pinMode(potPin, INPUT);
}

void loop()
{
  readMSGEQ7();

  clearLED();

//  delay(100);
  // display values of left channel on serial monitor
  for (band = 0; band < 7; band++)
  {
    Serial.print(left[band]);
    Serial.print(" ");
  }
  Serial.println(" - Left ");
  // display values of right channel on serial monitor
  for (band = 0; band < 7; band++)
  {
    Serial.print(right[band]);
    Serial.print(" ");
  }
  Serial.println(" - Right" );

  graph(0, left[0]);
  graph(1, left[1]);
  graph(2, left[2]);
  graph(3, left[3]);
  graph(4, left[4]);
  graph(5, left[5]);
  graph(6, left[6]);

  FastLED.show();

  spec(colpin1, left[0]);
  spec(colpin2, left[1]);
  spec(colpin3, left[2]);
  spec(colpin4, left[3]);
  spec(colpin5, left[4]);
  spec(colpin6, left[5]);
  spec(colpin7, left[6]);

  /*
    spec(colpin1, right[0]);
    spec(colpin2, right[1]);
    spec(colpin3, right[2]);
    spec(colpin4, right[3]);
    spec(colpin5, right[4]);
    spec(colpin6, right[5]);
    spec(colpin7, right[6]);
  */
}

void graph(int col, int height)
{
  if (height > 75)
  {
    leds[matrix[col][0]] = CRGB::Red;
  }
  if (height > 100)
  {
    leds[matrix[col][1]] = CRGB::Red;
  }
  if (height > 150)
  {
    leds[matrix[col][2]] = CRGB::Red;
  }
  if (height > 225)
  {
    leds[matrix[col][3]] = CRGB::Red;
  }
  if (height > 300)
  {
    leds[matrix[col][4]] = CRGB::Red;
  }
  if (height > 400)
  {
    leds[matrix[col][5]] = CRGB::Red;
  }
  if (height > 525)
  {
    leds[matrix[col][6]] = CRGB::Red;
  }
//  FastLED.show();
}

void clearLED()
{
  for (int i = 0; i < NUM_LEDS; i++) 
  {
    leds[i] = CRGB::Black;
  }
//  FastLED.show();
}

void readMSGEQ7()
// Function to read 7 band equalizers
{
  digitalWrite(res, HIGH);
  digitalWrite(res, LOW);
  for (band = 0; band < 7; band++)
  {
    digitalWrite(strobe, LOW); // strobe pin on the shield - kicks the IC up to the next band
    delayMicroseconds(30); //
    left[band] = analogRead(0); // store left band reading
    right[band] = analogRead(1); // ... and the right
    digitalWrite(strobe, HIGH);
  }
}

// function that lights of a column depending on height
void spec(int col, int height)
{
  digitalWrite(colpin1, HIGH);
  digitalWrite(colpin2, HIGH);
  digitalWrite(colpin3, HIGH);
  digitalWrite(colpin4, HIGH);
  digitalWrite(colpin5, HIGH);
  digitalWrite(colpin6, HIGH);
  digitalWrite(colpin7, HIGH);

  digitalWrite(rowpin1, LOW);
  digitalWrite(rowpin2, LOW);
  digitalWrite(rowpin3, LOW);
  digitalWrite(rowpin4, LOW);
  digitalWrite(rowpin5, LOW);

  digitalWrite(col, LOW);

  //for height
  if (height > 73)
  {
    digitalWrite(rowpin1, HIGH);
  }

  if (height > 80)
  {
    digitalWrite(rowpin2, HIGH);
  }

  if (height > 100)
  {
    digitalWrite(rowpin3, HIGH);
  }

  if (height > 120)
  {
    digitalWrite(rowpin4, HIGH);
  }

  if (height > 140)
  {
    digitalWrite(rowpin5, HIGH);
  }

  delay(2);
}

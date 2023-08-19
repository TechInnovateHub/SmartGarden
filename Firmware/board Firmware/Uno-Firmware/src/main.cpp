#include <Arduino.h>

#include <ThreeWire.h>  
#include <RtcDS1302.h>

#include "Wire.h"  
#include "LiquidCrystal_I2C.h" 

#define TEST_PROTOTYPE

const int flowSensorPin {2};
static double flowRate {};
volatile static int count {}; // limited to this file
static double priceOfWaterUsed {};

const int pumpRelayPin {4};
const int led {5};
const int solenoidPin {6};

const int soilMoisturePin {A0};

const int tooWetSoil{500};
const int tooDrySoil{300};

// function prototypes
void waterPlant();
void notWaterPlant();

void flow();
void flowSensorCount();
void flowSensorNotCount();
void calculatePrice();

LiquidCrystal_I2C lcd(0x27,20,4);  

void setup() {
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Hello, There!");
  lcd.setCursor(0,1);
  lcd.print("LOADING!");
  //  lcd.setCursor(0,2);
  // lcd.print("Arduino LCM IIC 2004");
  //  lcd.setCursor(0,3);
  // lcd.print("Power By Manasseh!");

  pinMode(pumpRelayPin, OUTPUT);
  pinMode(solenoidPin, OUTPUT);
  pinMode(led, OUTPUT);

  #ifdef TEST_PROTOTYPE
  Serial.begin(9600);
  #endif

  pinMode(flowSensorPin, INPUT);
  attachInterrupt(0, flow, RISING);

  lcd.clear(); 
}

void loop() {
  int soilMoistureValue {analogRead(soilMoisturePin)};

  #ifdef TEST_PROTOTYPE
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  #endif

  if (soilMoistureValue > tooWetSoil) 
  {
    notWaterPlant();
    delay(300);
  } 
  else if (soilMoistureValue < tooDrySoil) 
  {
    waterPlant();
    delay(300);
  } 
  else 
  {
    #ifdef TEST_PROTOTYPE
    Serial.println("Moisture is OK, don't water");
    #endif
    notWaterPlant();
    delay(300);
  }

  delay(300);
}

void waterPlant()
{
  #ifdef TEST_PROTOTYPE
  Serial.println("Too dry, water");
  #endif
  digitalWrite(pumpRelayPin, HIGH);
  digitalWrite(solenoidPin, HIGH);
  digitalWrite(led, HIGH);
  flowSensorCount();
  void calculatePrice();
  
}

void notWaterPlant()
{
  #ifdef TEST_PROTOTYPE
  Serial.println("Too wet, don't water");
  #endif
  digitalWrite(pumpRelayPin, LOW);
  digitalWrite(solenoidPin, LOW);
  digitalWrite(led, LOW);
  flowSensorNotCount();
}

void flow()
{
  count++;
}

void flowSensorCount()
{
  count = 0;
  interrupts();
}

void flowSensorNotCount()
{
  noInterrupts();
}

void calculatePrice()
{
  //calculation for the YF-S201 flow sensor (for this sensor only)
  flowRate = (count * 2.25);  //Take counted pulses all through when the watering process and multiply by 2.25mL
  flowRate = flowRate * 60;   //Convert seconds to minutes, giving you mL / Minute
  flowRate = flowRate * 1000; // //Convert mL to Liters, giving you Liters / Minute

  // calculate price of water used in Kenyan shs
  /*
    1L = i cubic metre 
    flowRate per 1 cubic metre = 60 Kshs
    price will be in Kshs per minute;
  */
  priceOfWaterUsed = flowRate * 60; 
}
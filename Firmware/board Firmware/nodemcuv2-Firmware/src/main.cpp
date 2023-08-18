#include <Arduino.h>
#include <cassert>

#define TEST_PROTOTYPE

const int pumpRelayPin {D2};
const int soilMoisturePin {A0};
const int tooWetSoil{500};
const int tooDrySoil{300};

// function prototypes
void waterPlant(int, int);
void notWaterPlant(int, int);

void setup() {
  pinMode(pumpRelayPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int soilMoistureValue {analogRead(soilMoisturePin)};
  #ifdef TEST_PROTOTYPE
  Serial.print("Soil Moisture: ");
  Serial.println(soilMoistureValue);
  #endif

  if (soilMoistureValue > tooWetSoil) 
  {
    notWaterPlant(soilMoistureValue, tooWetSoil);
    delay(300);
  } 
  else if (soilMoistureValue < tooDrySoil) 
  {
    waterPlant(soilMoistureValue , tooWetSoil);
    delay(300);
  } 
  else 
  {
    #ifdef TEST_PROTOTYPE
    Serial.println("Moisture is OK, don't water");
    #endif
    notWaterPlant(soilMoistureValue, tooDrySoil);
  }

  delay(300);
}

void waterPlant(int soilMoistureValue, int tooWetSoil)
{
  assert(soilMoistureValue < tooDrySoil); // safety feature(confirmation)
  #ifdef TEST_PROTOTYPE
  Serial.println("Too dry, water");
  #endif
  digitalWrite(pumpRelayPin, HIGH);
}

void notWaterPlant(int soilMoistureValue, int tooWetSoil)
{
  assert(soilMoistureValue > tooWetSoil);
  #ifdef TEST_PROTOTYPE
  Serial.println("Too wet, don't water");
  #endif
  digitalWrite(pumpRelayPin, LOW);
}
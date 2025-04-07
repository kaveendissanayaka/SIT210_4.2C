#include <Wire.h>
#include <BH1750.h>

const int buttonPin = 2;
const int led1Pin = 6;
const int led2Pin = 7;

volatile bool led1State = false;
bool led2State = false;

BH1750 lightMeter;
const int LIGHT_THRESHOLD = 50; // adjust to your environment

unsigned long lastLightCheck = 0;
const unsigned long lightInterval = 200; // ms

void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  Serial.begin(9600);
  while (!Serial);

  Wire.begin();
  if (lightMeter.begin()) {
    Serial.println("BH1750 initialized");
  } else {
    Serial.println("Error initializing BH1750");
  }

  attachInterrupt(digitalPinToInterrupt(buttonPin), handleButtonInterrupt, FALLING);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastLightCheck >= lightInterval) {
    lastLightCheck = currentMillis;
    checkLightLevel();
  }
}

void handleButtonInterrupt() {
  led1State = !led1State;
  digitalWrite(led1Pin, led1State);
  Serial.println("Button Pressed: Toggled LED1");
}

void checkLightLevel() {
  float lux = lightMeter.readLightLevel();
  Serial.print("Lux: ");
  Serial.println(lux); // This shows lux at every 200ms

  if (lux > LIGHT_THRESHOLD) {
    led2State = !led2State;
    digitalWrite(led2Pin, led2State);
    Serial.println("Light Sensor Triggered -> Toggled LED2");
  }
}

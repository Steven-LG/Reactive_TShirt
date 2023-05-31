#include <OneWire.h>
#include <DallasTemperature.h>

// Tira led
#define R_COLOR_PIN 8
#define G_COLOR_PIN 9
#define B_COLOR_PIN 10
#define STRIP_LED_DELAY 20

// Sensor Temperatura
#define SHIRT_TEMP_SENSOR_PIN 2
OneWire oneWire(SHIRT_TEMP_SENSOR_PIN);
DallasTemperature shirtTempSensor(&oneWire);
DeviceAddress insideThermometer;

// Sensor Temperatura Circuito
#define CIRCUIT_TEMP_SENSOR_PIN A2

// Sensor Microfono
#define MIC_SENSOR_PIN A0
int mic;
const int BASELINE = 337;
int amplitude;
const int MAX_AMPLITUDE = 60;
const int MAX_COLOR_GRADIENTS = 3;
const int DELTA = MAX_AMPLITUDE / (MAX_COLOR_GRADIENTS + 1);
const int THRESH_1 = DELTA;
const int THRESH_2 = THRESH_1 + DELTA;
const int THRESH_3 = THRESH_2 + DELTA;
const int THRESH_4 = THRESH_3 + DELTA;
const int THRESH_5 = THRESH_4 + DELTA;

// Sensor Voltaje
#define VOLTAGE_SENSOR_PIN A1
const int MAX_VOLTAGE = 250000;  //25V
int digitalLecture;             
float voltage;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //printShirtTempSensorInfo();
  RGBColor(0, 0, 255);
}

void printShirtTempSensorInfo() {
  Serial.print("Searching for temperature sensor...");
  shirtTempSensor.begin();

  Serial.print("Found ");
  Serial.print(shirtTempSensor.getDeviceCount(), DEC);
  Serial.println(" sensors");

  Serial.print("shirtTempSensor power is ");
  (shirtTempSensor.isParasitePowerMode())
    ? Serial.println("ACTIVE")
    : Serial.println("NOT ACTIVE");

  if (!shirtTempSensor.getAddress(insideThermometer, 0))
    Serial.println("Not found shirtTempSensor address.");

  printAddress(insideThermometer);
  Serial.println();

  shirtTempSensor.setResolution(insideThermometer, 9);

  Serial.print("shirtTempSensor response: ");
  Serial.print(shirtTempSensor.getResolution(insideThermometer), DEC);
  Serial.println();
}

void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

void loop() {

  digitalLecture = analogRead(VOLTAGE_SENSOR_PIN);
  voltage = map(digitalLecture, 0, 1023, 0, MAX_VOLTAGE) / 1000.0;

  mic = analogRead(MIC_SENSOR_PIN);
  amplitude = abs(mic - BASELINE);
  // Serial.println(amplitude);
  //Serial.print(amplitude);
  //Serial.print(" \t ");
  //Serial.println(voltage);

  // showInnerTemperature();
  //printCircuitTemperature();
  changeStripColor(amplitude);
 
}

void printCircuitTemperature(){
  int value = analogRead(A2);
  float volts = (value * 5) / 1023.0;
  float celsius = volts * 100;
  Serial.print(celsius);
  Serial.print(" C");
  delay(1000);
}

void changeStripColor(int amp) {
  if (amp > THRESH_5) {
    RGBColor(255, 165, 0);  // naranja claro
    RGBColor(255, 79, 0);   // naranja oscuro
    Serial.println("Tresh_5");

  } else if (amp > THRESH_4) {
    RGBColor(255, 79, 0);  // naranja oscuro
    RGBColor(255, 0, 0);   // rojo oscuro
    Serial.println("Tresh_4");

  } else if (amp > THRESH_3) {
    RGBColor(255, 0, 0);  // rojo oscuro
    RGBColor(200, 0, 0);  // rojo medio
    Serial.println("Tresh_3");

  } else if (amp > THRESH_2) {
    RGBColor(200, 0, 0);  // rojo medio
    RGBColor(150, 0, 0);  // rojo claro
    Serial.println("Tresh_2");

  } else if (amp > THRESH_1) {
    RGBColor(150, 0, 0);  // rojo claro
    RGBColor(0, 0, 0);    // apaga los LEDs
    Serial.println("Tresh_1");
  }
}

void showInnerTemperature() {
  Serial.print("Requesting temperature.");
  shirtTempSensor.requestTemperatures();
  Serial.println("FINISHED");

  float currentTemperature = shirtInnerTemperature(insideThermometer);
  if (currentTemperature < 38) {
    Serial.println("LOWER");
  } else {
    Serial.println("HIGHIER");
  }
}

float shirtInnerTemperature(DeviceAddress deviceAddress) {
  float tempInCelsius = shirtTempSensor.getTempC(deviceAddress);
  if (tempInCelsius == DEVICE_DISCONNECTED_C) {
    Serial.println("Error: Couldn't read shirtTempSensor");
    return;
  }

  Serial.print("ShirtInnerTemperature: \n" + String(tempInCelsius) + "C \t" + String(DallasTemperature::toFahrenheit(tempInCelsius)) + "F \n");
  return tempInCelsius;
}


void RGBColor(int Rcolor, int Gcolor, int Bcolor) {
  analogWrite(R_COLOR_PIN, Rcolor);
  analogWrite(G_COLOR_PIN, Gcolor);
  analogWrite(B_COLOR_PIN, Bcolor);

  delay(STRIP_LED_DELAY);
}

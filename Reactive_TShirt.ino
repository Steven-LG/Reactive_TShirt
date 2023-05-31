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

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

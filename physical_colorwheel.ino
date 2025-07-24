#include <math.h>

// RGB light
const int REDPIN = 11;
const int GREENPIN = 10;
const int BLUEPIN = 9;

// Joystick
const int XAXIS = A0;
const int YAXIS = A1;
const int RESPONSE_DELAY = 5; // 5ms response time
const long MAX_SENSOR_VALUE = 1023;
const long MAX_RGB_VALUE = 255;

void setup() {
  // put your setup code here, to run once:
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
}

void loop() {
  int xReading = analogRead(XAXIS);
  int yReading = analogRead(YAXIS);

  mapAxisValueToRGB(xReading, yReading);
  Serial.print("x-axis: ");
  Serial.println(xReading);

  Serial.print("y-axis: ");
  Serial.println(yReading);
  delay(RESPONSE_DELAY);
}

//Set RGB colors
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(REDPIN, redValue);
  analogWrite(GREENPIN, greenValue);
  analogWrite(BLUEPIN, blueValue);
}

// Convert hue (0–360), saturation (0–1), value (0–1) into 0–255 RGB
void hsvToRgb(float h, float s, float v, uint8_t &outR, uint8_t &outG, uint8_t &outB) {
  float c = v * s;
  float hp = h / 60.0;
  float x = c * (1.0 - fabs(fmod(hp, 2.0) - 1.0));
  float r1, g1, b1;

  if      (0.0 <= hp && hp < 1.0) { r1 = c; g1 = x; b1 = 0; }
  else if (1.0 <= hp && hp < 2.0) { r1 = x; g1 = c; b1 = 0; }
  else if (2.0 <= hp && hp < 3.0) { r1 = 0; g1 = c; b1 = x; }
  else if (3.0 <= hp && hp < 4.0) { r1 = 0; g1 = x; b1 = c; }
  else if (4.0 <= hp && hp < 5.0) { r1 = x; g1 = 0; b1 = c; }
  else                             { r1 = c; g1 = 0; b1 = x; }

  float m = v - c;
  outR = (r1 + m) * 255;
  outG = (g1 + m) * 255;
  outB = (b1 + m) * 255;
}

void mapAxisValueToRGB(int x, int y) {
  // compute vector from center
  float dx = x - 512.0;
  float dy = y - 512.0;

  // angle in radians then degrees, 0°→right, +CCW
  float angle = atan2(dy, dx) * 180.0 / M_PI;
  if (angle < 0) angle += 360.0;

  // use full saturation & brightness
  uint8_t r, g, b;
  hsvToRgb(angle, 1.0, 1.0, r, g, b);

  setColor(r, g, b);
}


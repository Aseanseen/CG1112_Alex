/**************************************************************************************************
 * lightsensor.h
 * Adafruit_TCS34725
 * Remember to include the Adafruit_TCS34725 library from Manage Libraries side
 * https://github.com/adafruit/Adafruit_TCS34725
**************************************************************************************************/

/*static uint16_t rawRed, rawGreen, rawBlue, rawClr;
static uint8_t red, green, blue;
static uint16_t lowRed = 0, highRed = 65535; //255
static uint16_t lowGreen = 0, highGreen = 65535;
static uint16_t lowBlue = 0, highBlue = 65535;*/

static uint16_t rawRed, rawGreen, rawBlue, rawClr;
static uint32_t red, green, blue, clr;

//calibrated values
static uint32_t lowRed = 74, highRed = 272;
static uint32_t lowGreen = 74, highGreen = 303;
static uint32_t lowBlue = 51, highBlue = 239;

//default
/*static uint32_t lowRed = 0, highRed = 1020;
static uint32_t lowGreen = 0, highGreen = 1020;
static uint32_t lowBlue = 0, highBlue = 1020;*/
static bool isRed;

//Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_16X);
Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

void initSensor() {
  sensor.begin();
  //sensor.setIntLimits(6553, 65535);
  sensor.clearInterrupt();
  sensor.setInterrupt(true);
}

void readColor() {
  message = "Reading color (RGB)\n";
  sendMsgAuto();

  red = 0; green = 0; blue = 0;
  for (int i = 0; i < 4; i++) { //grab 4 tries 
    sensor.setInterrupt(false);
    delay(165);
    sensor.getRawData(&rawRed, &rawGreen, &rawBlue, &rawClr);
    sensor.setInterrupt(true);
    delay(10);
    red += rawRed;
    green += rawGreen;
    blue += rawBlue;
  }
  
  /*sensor.setInterrupt(false);
  delay(170);
  sensor.getRawData(&rawRed, &rawGreen, &rawBlue, &rawClr);
  sensor.setInterrupt(true);
  red = max(0, min((((uint32_t)rawRed - lowRed) << 8) / (highRed - lowRed), 255));
  green = max(0, min((((uint32_t)rawGreen - lowGreen) << 8) / (highGreen - lowGreen), 255));
  blue = max(0, min((((uint32_t)rawBlue - lowBlue) << 8) / (highBlue - lowBlue), 255));
  //red = max(0, min((((uint32_t)rawRed << 8) / rawClr - lowRed << 8) / (highRed - lowRed), 255));
  //green = max(0, min((((uint32_t)rawGreen << 8) / rawClr - lowGreen << 8) / (highGreen - lowGreen), 255));
  //blue = max(0, min((((uint32_t)rawBlue << 8) / rawClr - lowBlue << 8) / (highBlue - lowBlue), 255));*/

  //determine red / green
  red = ((red - lowRed) << 8) / (highRed - lowRed);
  green = ((green - lowGreen) << 8) / (highGreen - lowGreen);
  blue = ((blue - lowBlue) << 8) / (highBlue - lowBlue);
  isRed = red > green ? true : false;

  if (isRed) message = "Color of object is RED";
  else message = "Color of object is GREEN";
  sendMsgAuto();
  
  itoa(red, message, 10);
  sendMsgAuto();
  itoa(green, message, 10);
  sendMsgAuto();
  itoa(blue, message, 10);
  sendMsgAuto();
  message = "Reading complete";
  sendMsgAuto();
}


void calibrateLS() {
  lowRed = 0, highRed = 1020;
  lowGreen = 0, highGreen = 1020;
  lowBlue = 0, highBlue = 1020;
  message = "Light Sensor Calibration";
  sendMsgAuto();  
  message = "Prep black obj. in 3";
  sendMsgAuto();
  _delay_ms(1000);
  message = "Prep black obj. in 2";
  sendMsgAuto();
  _delay_ms(1000);
  message = "Prep black obj. in 1";
  sendMsgAuto();
  _delay_ms(1000);
  
  readColor();
  uint16_t tmpLowRed = red, tmpLowGreen = green, tmpLowBlue = blue;

  message = "Black color calibration complete";
  sendMsgAuto();
  
  message = "Prep white obj. in 3";
  sendMsgAuto();
  _delay_ms(1000);
  message = "Prep white obj. in 2";
  sendMsgAuto();
  _delay_ms(1000);
  message = "Prep white obj. in 1";
  sendMsgAuto();
  _delay_ms(1000);
  
  readColor();
  lowRed = tmpLowRed; lowGreen = tmpLowGreen, lowBlue = tmpLowBlue;
  highRed = red; highGreen = green; highBlue = blue;

  message = "White color calibration complete";
  sendMsgAuto();
}

void setupLS() {
  initSensor();
  //calibrateLS();
}

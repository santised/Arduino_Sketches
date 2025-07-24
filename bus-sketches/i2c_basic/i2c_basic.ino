/*
  0b10000000 - "1" is the C0 bit

  C0 == "0" indicates sending last control byte, only data bytes follow
  * If you only send one control byte then it should be zero
  C0 == "1" indicates the next two bytes are data byte and another control byte.....

  0b01000000 - "1" is the D/C bit

  D/C == "1" indicates the databyte is for RAM operation
  D/C == "0" indicates the databyte is for command operation
*/
#include <Wire.h>

  

const int oledAddrOpen = 0x3C; 
const int oledAddrClosed = 0x3D; 
const int rowStart = 0x00; 
const int rowEnd = 0x3F; 
const int colStart = 0x00; 
const int colEnd = 0x1F; 
const int mono = 0x01;
int led = LED_BUILTIN;

enum commandList
{
  commandControlByte = 0x00,
  anotherControlByte = 0xC0,
  controlDataByteFollow = 0x80,
  ramControlByte = 0x40,
  //Two byte command - command, start, stop
  commandRowStartEnd = 0x22,  
  //Two byte command - command, start, stop
  commandColStartEnd = 0x21,  
  commandStartLine = 0xA2,
  commandContrastControl = 0x81,
  commandDisplayOn = 0xAF,  
  commandDisplayOff = 0xAE,  
  commandPanelID = 0xE1,  
  commandDriverID = 0xE2, //BUSY+ON/OFF+0x60
  commandGrayMono = 0xAC,
  commandSetReMapDown = 0xA0, //default 
  commandCommonOutScan = 0xC8, // scan COM[n-1] to COM0 default = 0xC0 
  commandDisplayRotation = 0xA3,
  commandDisableEntireDisplay = 0xA4,
  commandNormalDisplay = 0xA6, //default 
  commandMultiplexRatio = 0xA8,
  commandDisplayOffset = 0xD3,
  commandDisplayDivideRatio = 0xD5,
  commandDischargeFront = 0x93,
  commandDischargeBack = 0xD8,
  commandPreCharge = 0xD9,
  commandSEGpads = 0xDA,
  commandVCOMDeselectLevel = 0xDB, 
  commandExternalIREF = 0xAD,
}; 

enum manufactureSettings
{
  monoMode = 0x01, //grayscale by default "0x00"
  manRowStart = 0x00, //default
  manColStart = 0x00, //default
  manRowEnd = 0x3F, //default
  manColEnd = 0x1F, //default
  manDisplayStart = 0x00, //default
  horizontalAddressing = 0x00, //default 
  contrast = 0xC8, //default = 0x80, upper end = 0xFF
  rotateDisplayNinety = 0x01, //Default is 0 degrees
  multiplexRatio = 0x7F, //default is 0x9F
  displayOffset = 0x10, // default is 0x00
  divideRatio = 0x00, //default
  dischargeFront = 0x02,// default is 0x02
  dischargeBack = 0x02,//default
  preCharge = 0x1F,//0x1F is default
  segPads = 0x00,//default 
  vcomDeselect = 0x3F,//default 
  externalIREF = 0x02
};

void setup()
{
  Serial.begin(115200); // Default config settings
  //Serial1.begin(115200, SERIAL_8N1); // Default config settings - ESP doesn't like this.
  //Serial2.begin(115200, SERIAL_8N1); // Default config settings
  Wire.begin();

  while(!Serial) // Circle the void until the monitor opens
      ;

  Serial.println("I2C Sketch test.");

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
    
  //Wire.setClock(10000); //Low mode
  //Wire.setClock(4000000); //Fast mode
  //Wire.setClock(1000000); //Fast mode plus
  //Wire.setClock(3400000); //High speed mode
  Wire.beginTransmission(oledAddrOpen);
  int temp = Wire.endTransmission(oledAddrOpen);
  Serial.println("Result (want '0'): ");
  Serial.println(temp); 

  // Sanity Check.
  getDriverID();
  //setupOLED();
  manufactureSetup();
  //fillSquare();

  Serial.println("Write finished, Looping.");
  writeCommand(commandDisableEntireDisplay);
  //writeCommand(commandDisplayOff);
  Serial.print("Checking if display is on: ");
  getDriverID();
}

void loop()
{
    delay(1000);
}

void getDriverID()
{
  writeCommand(commandDriverID);

  Wire.requestFrom(oledAddrOpen, 1);

  while(Wire.available())
  {
    Serial.print("0x");
    Serial.println(Wire.read(), HEX);
  }

  Wire.endTransmission();
  delay(1000);
}
void setupOLED()
{
  Wire.beginTransmission(oledAddrOpen);
  Wire.write(commandControlByte);
  Wire.write(commandDisplayOn);
  Wire.endTransmission();
}

void manufactureSetup()
{
  writeCommand(commandDisplayOff);
  writeCommandParameter(commandContrastControl, contrast);
  writeCommandParameter(commandGrayMono, monoMode);
  writeCommand(commandCommonOutScan);
  writeCommand(commandDisableEntireDisplay);
  writeCommandParameter(commandMultiplexRatio, multiplexRatio);
  writeCommandParameter(commandDisplayOffset, displayOffset);
  writeCommandParameter(commandDischargeFront, dischargeFront);
  writeCommandParameter(commandDischargeBack, dischargeBack);
  writeCommandParameter(commandPreCharge, preCharge);
  writeCommand(commandDisplayOn);
}

void fillSquare()
{

  Wire.beginTransmission(oledAddrOpen);
  Wire.write(commandControlByte);
  // data pointer
  Wire.write(rowStart);
  Wire.endTransmission();

  Wire.beginTransmission(oledAddrOpen);
  // write to RAM
  Wire.write(ramControlByte);
  for( int i = rowStart; i < (rowEnd - rowStart); i++ ) 
  {
    for (int j = colStart; j < (colEnd - colStart); j++) 
    {
      Wire.write(0x0F);
    }
  }
  Wire.endTransmission();
}

void writeCommand(uint8_t command)
{
  Wire.beginTransmission(oledAddrOpen);
  Wire.write(controlDataByteFollow);
  Wire.write(command);
  Wire.endTransmission();
}

void writeCommandParameter(uint8_t command, uint8_t parameter)
{
  Wire.beginTransmission(oledAddrOpen);
  Wire.write(commandControlByte);
  Wire.write(command);
  Wire.write(parameter);
  Wire.endTransmission();
}

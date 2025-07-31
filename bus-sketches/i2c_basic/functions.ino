const int rowStart = 0x00; 
const int rowEnd = 0x3F; 
const int colStart = 0x00; 
const int colEnd = 0x3F; 
const int mono = 0x01;

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
  commandStartRow = 0xB0,
  commandStartColLow = 0x00,
  commandStartColHigh = 0x10,
  commandStartLine = 0xA2,

  commandContrastControl = 0x81,
  commandHorizAddressing = 0x20,
  commandDisplayOn = 0xAF,  
  commandDisplayOff = 0xAE,  
  commandPanelID = 0xE1,  
  commandDriverID = 0xE2, //BUSY+ON/OFF+0x60
  commandGrayMono = 0xAC,
  commandSetReMapDown = 0xA0, //default 
  commandCommonOutScan = 0xC8, // scan COM[n-1] to COM0 default = 0xC0 
  commandDisplayRotation = 0xA3,
  commandDisableEntireDisplay = 0xA4,
  commandEnableEntireDisplay = 0xA5,
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

void manufactureSetup()
{
  writeCommand(commandDisplayOff);
  setStartStop(commandRowStartEnd, rowStart, rowEnd);
  setStartStop(commandColStartEnd, colStart, colEnd);
  writeCommand(commandStartLine);
  writeCommandParameter(commandContrastControl, contrast);
  writeCommandParameter(commandGrayMono, monoMode);
  writeCommandParameter(commandHorizAddressing, horizontalAddressing);
  writeCommand(commandSetReMapDown); 
  writeCommand(commandCommonOutScan);
  writeCommandParameter(commandDisplayRotation, rotateDisplayNinety); 
  writeCommand(commandDisableEntireDisplay);
  writeCommandParameter(commandMultiplexRatio, multiplexRatio);
  writeCommandParameter(commandDisplayOffset, displayOffset);
  writeCommandParameter(commandDischargeFront, dischargeFront);
  writeCommandParameter(commandDischargeBack, dischargeBack);
  writeCommandParameter(commandPreCharge, preCharge);
  writeCommandParameter(commandSEGpads, segPads);
  writeCommandParameter(commandVCOMDeselectLevel, vcomDeselect);
  writeCommandParameter(commandExternalIREF, externalIREF);
  writeCommand(commandDisplayOn);
}

void fillSquare(uint8_t color)
{
  Wire.beginTransmission(oledAddrOpen);
  Wire.write(ramControlByte);
  for( int i = rowStart; i < 128; i++ ) 
  {
    for (int j = colStart; j < 128; j++) 
    {
      Wire.write(color);
    }
  }
  Wire.endTransmission();
}


void writeCommand(uint8_t command)
{
  Wire.beginTransmission(oledAddrOpen);
  Wire.write(commandControlByte);
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

void setStartStop(uint8_t command, uint8_t start, uint8_t stop)
{

  Wire.beginTransmission(oledAddrOpen);
  Wire.write(commandControlByte);
  Wire.write(command);
  Wire.write(start);
  Wire.write(stop);
  Wire.endTransmission();
}

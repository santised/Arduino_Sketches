const int rowStart = 0x00; 
const int rowEnd = 0x3F; 
const int colStart = 0x00; 
const int colEnd = 0x1F; 
const int mono = 0x01;

enum commandList
{
  //Two byte command - command, start, stop
  commandRowStartEnd = 0x22,  
  //Two byte command - command, start, stop
  commandColStartEnd = 0x21,  
  commandStartRow = 0xB0,
  commandStartColLow = 0x00,
  commandStartColHigh = 0x10,
  commandStartLine = 0xA2,

  commandContrastControl = 0x0F,
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
void manufactureSetup()
{
  writeCommand(commandDisplayOff);
  setStartStop(commandRowStartEnd, rowStart, rowEnd);
  setStartStop(commandColStartEnd, colStart, colEnd);
  writeCommandParameter(commandStartLine, manDisplayStart);
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

void drawPixel(uint8_t pixelWidth)
{
  digitalWrite(cs, LOW);
  digitalWrite(deeCee, HIGH);
  SPI.beginTransaction(oledSettings);
  SPI.transfer(pixelWidth);
  digitalWrite(cs, HIGH);
  SPI.endTransaction();
}
void writeCommand(uint8_t command)
{
  digitalWrite(cs, LOW);
  digitalWrite(deeCee, LOW);
  SPI.beginTransaction(oledSettings);
  SPI.transfer(command);
  digitalWrite(cs, HIGH);
  SPI.endTransaction();
}

void writeCommandParameter(uint8_t command, uint8_t parameter)
{
  digitalWrite(cs, LOW);
  digitalWrite(deeCee, LOW);
  SPI.beginTransaction(oledSettings);
  SPI.transfer(command);
  SPI.transfer(parameter);
  digitalWrite(cs, HIGH);
  SPI.endTransaction();
}


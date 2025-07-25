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
  setStartStop(commandRowStartEnd, rowStart, rowEnd);
  setStartStop(commandColStartEnd, colStart, colEnd);
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

void wifiSettings(){

  // SPIFFS.open() supports all Stream methods
  //https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
  File configFile = SPIFFS.open(configPath, "r");
  if(!configFile)
    Serial.println("Could not open file.");
  
  uint16_t size = configFile.size(); 

  // Do I want to allocate size according to the size above?
  // Hard code the bytes according to ArduinoJSON web page.
  const size_t capacity = JSON_ARRAY_SIZE(4) + JSON_OBJECT_SIZE(3) + 100;
  DynamicJsonDocument doc(capacity); 

  DeserializationError err = deserializeJson(doc, configFile);
 
  if(err) {
    Serial.print("Deserialization error:");
    Serial.println(err.c_str());
  }

  doc["SSID"].as<String>().toCharArray(ssid, 30);
  doc["Password"].as<String>().toCharArray(password, 30);

  configFile.close();

}


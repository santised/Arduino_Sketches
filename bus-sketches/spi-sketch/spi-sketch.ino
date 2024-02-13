#include <SPI.h>
#include "SparkFun_LSM6DSV16X.h"

// SPI instance class call
SparkFun_LSM6DSV16X_SPI myLSM;

// Structs for X,Y,Z data
sfe_lsm_data_t accelData; 
sfe_lsm_data_t gyroData; 

// Set your chip select pin according to your setup. 
int chipSelect = 4;
int led = LED_BUILTIN;
// Used on transaction calls 
//SPISettings mySPISettings(14000000, MSBFIRST, SPI_MODE0); 

void setup()
{
    Serial.begin(115200); // Default config settings
    //Serial1.begin(115200, SERIAL_8N1); // Default config settings
    //Serial2.begin(115200, SERIAL_8N1); // Default config settings

    SPI.begin();

    while(!Serial) // Circle the void until the monitor opens
        ;

    Serial.println("SPI test sketch.");

    pinMode(chipSelect, OUTPUT);
	digitalWrite(chipSelect, HIGH);


	if(!myLSM.begin(chipSelect)) 
    {
        Serial.println("Did not begin.");
        while(1) 
        ;
	} 

    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);

    myLSM.deviceReset();

    // Wait for it to finish reseting
    while( !myLSM.getDeviceReset() ){ 
        delay(1);
    } 

    Serial.println("Reset.");
    Serial.println("Applying settings.");
    delay(100);

    // Accelerometer and Gyroscope registers will not be updated
    // until read.
    myLSM.enableBlockDataUpdate();

    // Set the output data rate and precision of the accelerometer
    myLSM.setAccelDataRate(LSM6DSV16X_ODR_AT_7Hz5);
    myLSM.setAccelFullScale(LSM6DSV16X_16g);

    // Set the output data rate and precision of the gyroscope
    myLSM.setGyroDataRate(LSM6DSV16X_ODR_AT_15Hz);
    myLSM.setGyroFullScale(LSM6DSV16X_2000dps);

    // Enable filter settling.
    myLSM.enableFilterSettling();

    // Turn on the accelerometer's filter and apply settings.
    myLSM.enableAccelLP2Filter();
    myLSM.setAccelLP2Bandwidth(LSM6DSV16X_XL_STRONG);

    // Turn on the gyroscope's filter and apply settings.
    myLSM.enableGyroLP1Filter();
    myLSM.setGyroLP1Bandwidth(LSM6DSV16X_GY_ULTRA_LIGHT);

    Serial.print("Ready.");
}

void loop()
{
    if( myLSM.checkStatus() )
    {
		myLSM.getAccel(&accelData);
		myLSM.getGyro(&gyroData);
		Serial.print("Accelerometer: ");
		Serial.print("X: ");
		Serial.print(accelData.xData);
		Serial.print(" ");
		Serial.print("Y: ");
		Serial.print(accelData.yData);
		Serial.print(" ");
		Serial.print("Z: ");
		Serial.print(accelData.zData);
		Serial.println(" ");
		Serial.print("Gyroscope: ");
		Serial.print("X: ");
		Serial.print(gyroData.xData);
		Serial.print(" ");
		Serial.print("Y: ");
		Serial.print(gyroData.yData);
		Serial.print(" ");
		Serial.print("Z: ");
		Serial.print(gyroData.zData);
		Serial.println(" ");
    }
}




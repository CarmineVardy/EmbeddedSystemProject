/*
 * sen15219.c
 *
 *  Created on: May 26, 2025
 *      Author: utente
 */

#include "sen15219.h"

void SparkFun_Bio_Sensor_Hub_init(SparkFun_Bio_Sensor_Hub* hub, int resetPin, int mfioPin) {

	 	hub->resetPin = resetPin;
	    hub->mfioPin = mfioPin;


	    if (resetPin >= 0) {
	        pinMode(resetPin, OUTPUT);
	    }

	    if (mfioPin >= 0) {
	        pinMode(mfioPin, OUTPUT);
	    }

	    hub->address = DEFAULT_ADDRESS;
}

int SparkFun_Bio_Sensor_Hub_begin(SparkFun_Bio_Sensor_Hub* hub){

	 _i2cPort = &wirePort;
	    //  _i2cPort->begin(); A call to Wire.begin should occur in sketch
	    //  to avoid multiple begins with other sketches.

	    if (resetPin >= 0)
	    {
	        _resetPin = resetPin;
	        pinMode(_resetPin, OUTPUT); // Set the pin as output
	    }

	    if (mfioPin >= 0)
	    {
	        _mfioPin = mfioPin;
	        pinMode(_mfioPin, OUTPUT); // Set the pin as output
	    }

	    if ((_resetPin < 0) || (_mfioPin < 0)) // Bail if the pins have still not been defined
	        return 0xFF;                       // Return ERR_UNKNOWN

	    digitalWrite(_mfioPin, HIGH);
	    digitalWrite(_resetPin, LOW);
	    delay(10);
	    digitalWrite(_resetPin, HIGH);
	    delay(1000);
	    pinMode(_mfioPin, INPUT_PULLUP); // To be used as an interrupt later

	    uint8_t responseByte = readByte(READ_DEVICE_MODE, 0x00); // 0x00 only possible Index Byte.
	    return responseByte;

}


int SparkFun_Bio_Sensor_Hub_configSensorBpm(SparkFun_Bio_Sensor_Hub* hub, uint8_t mode){

	uint8_t statusChauf; // Our status chauffeur
	    if (mode == MODE_ONE || mode == MODE_TWO)
	    {
	    }
	    else
	        return INCORR_PARAM;

	    statusChauf = setOutputMode(SENSOR_AND_ALGORITHM); // Data and sensor data
	    if (statusChauf != SFE_BIO_SUCCESS)
	        return statusChauf;

	    statusChauf = setFifoThreshold(0x01); // One sample before interrupt is fired to the MAX32664
	    if (statusChauf != SFE_BIO_SUCCESS)
	        return statusChauf;

	    statusChauf = max30101Control(ENABLE); // Enable Sensor.
	    if (statusChauf != SFE_BIO_SUCCESS)
	        return statusChauf;

	    statusChauf = maximFastAlgoControl(mode); // Enable algorithm
	    if (statusChauf != SFE_BIO_SUCCESS)
	        return statusChauf;

	    _userSelectedMode = mode;
	    _sampleRate = readAlgoSamples();

	    delay(1000);
	    return SFE_BIO_SUCCESS;


}

int SparkFun_Bio_Sensor_Hub_setPulseWidth(SparkFun_Bio_Sensor_Hub* hub, int width){
	uint8_t bits;
	    uint8_t regVal;

	    // Make sure the correct pulse width is selected.
	    if (width == 69)
	        bits = 0;
	    else if (width == 118)
	        bits = 1;
	    else if (width == 215)
	        bits = 2;
	    else if (width == 411)
	        bits = 3;
	    else
	        return INCORR_PARAM;

	    // Get current register value so that nothing is overwritten.
	    regVal = readRegisterMAX30101(CONFIGURATION_REGISTER);
	    regVal &= PULSE_MASK;                                  // Mask bits to change.
	    regVal |= bits;                                        // Add bits
	    writeRegisterMAX30101(CONFIGURATION_REGISTER, regVal); // Write Register

	    return SFE_BIO_SUCCESS;

}

int SparkFun_Bio_Sensor_Hub_readPulseWidth(SparkFun_Bio_Sensor_Hub* hub){

	 uint8_t regVal;

	    regVal = readRegisterMAX30101(CONFIGURATION_REGISTER);
	    regVal &= READ_PULSE_MASK;

	    if (regVal == 0)
	        return 69;
	    else if (regVal == 1)
	        return 118;
	    else if (regVal == 2)
	        return 215;
	    else if (regVal == 3)
	        return 411;
	    else
	        return ERR_UNKNOWN;

}

int SparkFun_Bio_Sensor_Hub_setSampleRate(SparkFun_Bio_Sensor_Hub* hub, int samples){
	uint8_t bits;
	    uint8_t regVal;

	    // Make sure the correct sample rate was picked
	    if (sampRate == 50)
	        bits = 0;
	    else if (sampRate == 100)
	        bits = 1;
	    else if (sampRate == 200)
	        bits = 2;
	    else if (sampRate == 400)
	        bits = 3;
	    else if (sampRate == 800)
	        bits = 4;
	    else if (sampRate == 1000)
	        bits = 5;
	    else if (sampRate == 1600)
	        bits = 6;
	    else if (sampRate == 3200)
	        bits = 7;
	    else
	        return INCORR_PARAM;

	    // Get current register value so that nothing is overwritten.
	    regVal = readRegisterMAX30101(CONFIGURATION_REGISTER);
	    regVal &= SAMP_MASK;                                   // Mask bits to change.
	    regVal |= (bits << 2);                                 // Add bits but shift them first to correct position.
	    writeRegisterMAX30101(CONFIGURATION_REGISTER, regVal); // Write Register

	    return SFE_BIO_SUCCESS;

}

int SparkFun_Bio_Sensor_Hub_readSampleRate(SparkFun_Bio_Sensor_Hub* hub){

	uint8_t regVal;

	    regVal = readRegisterMAX30101(CONFIGURATION_REGISTER);
	    regVal &= READ_SAMP_MASK;
	    regVal = (regVal >> 2);

	    if (regVal == 0)
	        return 50;
	    else if (regVal == 1)
	        return 100;
	    else if (regVal == 2)
	        return 200;
	    else if (regVal == 3)
	        return 400;
	    else if (regVal == 4)
	        return 800;
	    else if (regVal == 5)
	        return 1000;
	    else if (regVal == 6)
	        return 1600;
	    else if (regVal == 7)
	        return 3200;
	    else
	        return ERR_UNKNOWN;

}

bioData SparkFun_Bio_Sensor_Hub_readSensorBpm(SparkFun_Bio_Sensor_Hub* hub){

	bioData libLedBpm;

	    if (_userSelectedMode == MODE_ONE)
	    {

	        readFillArray(READ_DATA_OUTPUT, READ_DATA, MAXFAST_ARRAY_SIZE + MAX30101_LED_ARRAY, bpmSenArr);

	        // Value of LED one....
	        libLedBpm.irLed = uint32_t(bpmSenArr[0]) << 16;
	        libLedBpm.irLed |= uint32_t(bpmSenArr[1]) << 8;
	        libLedBpm.irLed |= bpmSenArr[2];

	        // Value of LED two...
	        libLedBpm.redLed = uint32_t(bpmSenArr[3]) << 16;
	        libLedBpm.redLed |= uint32_t(bpmSenArr[4]) << 8;
	        libLedBpm.redLed |= bpmSenArr[5];

	        // -- What happened here? -- There are two uint32_t values that are given by
	        // the sensor for LEDs that do not exists on the MAX30101. So we have to
	        // request those empty values because they occupy the buffer:
	        // bpmSenArr[6-11].

	        // Heart rate formatting
	        libLedBpm.heartRate = (uint16_t(bpmSenArr[12]) << 8);
	        libLedBpm.heartRate |= (bpmSenArr[13]);
	        libLedBpm.heartRate /= 10;

	        // Confidence formatting
	        libLedBpm.confidence = bpmSenArr[14];

	        // Blood oxygen level formatting
	        libLedBpm.oxygen = uint16_t(bpmSenArr[15]) << 8;
	        libLedBpm.oxygen |= bpmSenArr[16];
	        libLedBpm.oxygen /= 10;

	        //"Machine State" - has a finger been detected?
	        libLedBpm.status = bpmSenArr[17];
	        return libLedBpm;
	    }

	    else if (_userSelectedMode == MODE_TWO)
	    {

	        readFillArray(READ_DATA_OUTPUT, READ_DATA, MAXFAST_ARRAY_SIZE + MAX30101_LED_ARRAY + MAXFAST_EXTENDED_DATA,
	                      bpmSenArrTwo);

	        // Value of LED one....
	        libLedBpm.irLed = uint32_t(bpmSenArrTwo[0]) << 16;
	        libLedBpm.irLed |= uint32_t(bpmSenArrTwo[1]) << 8;
	        libLedBpm.irLed |= bpmSenArrTwo[2];

	        // Value of LED two...
	        libLedBpm.redLed = uint32_t(bpmSenArrTwo[3]) << 16;
	        libLedBpm.redLed |= uint32_t(bpmSenArrTwo[4]) << 8;
	        libLedBpm.redLed |= bpmSenArrTwo[5];

	        // -- What happened here? -- There are two uint32_t values that are given by
	        // the sensor for LEDs that do not exists on the MAX30101. So we have to
	        // request those empty values because they occupy the buffer:
	        // bpmSenArrTwo[6-11].

	        // Heart rate formatting
	        libLedBpm.heartRate = (uint16_t(bpmSenArrTwo[12]) << 8);
	        libLedBpm.heartRate |= (bpmSenArrTwo[13]);
	        libLedBpm.heartRate /= 10;

	        // Confidence formatting
	        libLedBpm.confidence = bpmSenArrTwo[14];

	        // Blood oxygen level formatting
	        libLedBpm.oxygen = uint16_t(bpmSenArrTwo[15]) << 8;
	        libLedBpm.oxygen |= bpmSenArrTwo[16];
	        libLedBpm.oxygen /= 10;

	        //"Machine State" - has a finger been detected?
	        libLedBpm.status = bpmSenArrTwo[17];

	        // Sp02 r Value formatting
	        uint16_t tempVal = uint16_t(bpmSenArrTwo[18]) << 8;
	        tempVal |= bpmSenArrTwo[19];
	        libLedBpm.rValue = tempVal;
	        libLedBpm.rValue /= 10.0;

	        // Extended Machine State formatting
	        libLedBpm.extStatus = bpmSenArrTwo[20];

	        // There are two additional bytes of data that were requested but that
	        // have not been implemented in firmware 10.1 so will not be saved to
	        // user's data.
	        //
	        return libLedBpm;
	    }

	    else
	    {
	        libLedBpm.irLed = 0;
	        libLedBpm.redLed = 0;
	        libLedBpm.heartRate = 0;
	        libLedBpm.confidence = 0;
	        libLedBpm.oxygen = 0;
	        libLedBpm.status = 0;
	        libLedBpm.rValue = 0;
	        libLedBpm.extStatus = 0;
	        return libLedBpm;
	    }

}


/*
 * sen15219.h
 *
 *  Created on: May 26, 2025
 *      Author: utente
 */

//Oxygen
//Status
//HearthRate
//Confidence
#ifndef INC_SEN15219_H_
#define INC_SEN15219_H_

#define DEFAULT_ADDRESS 0x55

typedef struct {
    int resetPin;
    int mfioPin;
    uint8_t address;
} SparkFun_Bio_Sensor_Hub;

typedef struct {
    uint32_t irLed;
    uint32_t redLed;
    uint16_t heartRate;
    uint16_t oxygen;
    uint8_t status;
    float rValue;
    int8_t extStatus;
    uint8_t reserveOne;
    uint8_t resserveTwo;
} bioData;

void SparkFun_Bio_Sensor_Hub_init(SparkFun_Bio_Sensor_Hub* hub, int resetPin, int mfioPin);
int SparkFun_Bio_Sensor_Hub_begin(SparkFun_Bio_Sensor_Hub* hub);
int SparkFun_Bio_Sensor_Hub_configSensorBpm(SparkFun_Bio_Sensor_Hub* hub, uint8_t mode);
int SparkFun_Bio_Sensor_Hub_setPulseWidth(SparkFun_Bio_Sensor_Hub* hub, int width);
int SparkFun_Bio_Sensor_Hub_readPulseWidth(SparkFun_Bio_Sensor_Hub* hub);
int SparkFun_Bio_Sensor_Hub_setSampleRate(SparkFun_Bio_Sensor_Hub* hub, int samples);
int SparkFun_Bio_Sensor_Hub_readSampleRate(SparkFun_Bio_Sensor_Hub* hub);
bioData SparkFun_Bio_Sensor_Hub_readSensorBpm(SparkFun_Bio_Sensor_Hub* hub);




#endif /* INC_SEN15219_H_ */

#ifndef PACKET_H
#define PACKET_H

#include <Arduino.h>

struct IoTPacket
{
    uint32_t packetID;

    unsigned long timestamp;

    float temperature;
    float humidity;
    float pressure;
    float gas;

    float accelX;
    float accelY;
    float accelZ;

    int battery;

    int cpuLoad;

    uint8_t priority;

    uint8_t threatLevel;

    uint8_t aesMode;
};

#endif
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "Packet.h"

class SensorManager
{
public:

    void begin();

    IoTPacket readSensors();

private:

    float simulateTemperature();

    float simulateHumidity();

    float simulatePressure();

    float simulateGas();

    float simulateAccelX();

    float simulateAccelY();

    float simulateAccelZ();

    bool validatePacket(IoTPacket &packet);

    int packetCounter = 0;
};

#endif
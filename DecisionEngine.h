#ifndef DECISION_ENGINE_H
#define DECISION_ENGINE_H

#include "Packet.h"

class DecisionEngine
{
public:

    void begin();

    float calculateDecisionScore(const IoTPacket& packet);

    uint16_t selectAESMode(float score);

    uint8_t calculateBatteryScore(int battery);

    uint8_t calculateCPUScore(int cpu);

    uint8_t calculatePriorityScore(uint8_t priority);

    uint8_t calculateThreatScore(uint8_t threat);

};

#endif
#include "DecisionEngine.h"

void DecisionEngine::begin()
{

}

uint8_t DecisionEngine::calculateBatteryScore(int battery)
{
    if(battery<=30)
        return 20;

    if(battery<=75)
        return 60;

    return 100;
}

uint8_t DecisionEngine::calculateCPUScore(int cpu)
{
    if(cpu>80)
        return 20;

    if(cpu>=40)
        return 60;

    return 100;
}

uint8_t DecisionEngine::calculatePriorityScore(uint8_t priority)
{
    switch(priority)
    {
        case 1: return 30;
        case 2: return 60;
        case 3: return 100;
    }

    return 30;
}

uint8_t DecisionEngine::calculateThreatScore(uint8_t threat)
{
    switch(threat)
    {
        case 1: return 20;
        case 2: return 60;
        case 3: return 100;
    }

    return 20;
}

float DecisionEngine::calculateDecisionScore(const IoTPacket& packet)
{
    float battery =
        calculateBatteryScore(packet.battery);

    float cpu =
        calculateCPUScore(packet.cpuLoad);

    float priority =
        calculatePriorityScore(packet.priority);

    float threat =
        calculateThreatScore(packet.threatLevel);

    return
        0.35*battery+
        0.25*cpu+
        0.25*priority+
        0.15*threat;
}

uint16_t DecisionEngine::selectAESMode(float score)
{
    if(score<=40)
        return 128;

    if(score<=70)
        return 192;

    return 256;
}
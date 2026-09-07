#include "Comparator.h"

void Comparator::begin()
{
    decisionEngine.begin();
    aesManager.begin();
}

ComparisonReport Comparator::compare(IoTPacket &packet)
{
    ComparisonReport result;

    //--------------------------------------------------
    // WARM-UP (not measured)
    // Loads AES code into cache for all three variants
    //--------------------------------------------------
    IoTPacket warmupPacket = packet;
    aesManager.encryptPacket(warmupPacket, 128);
    aesManager.encryptPacket(warmupPacket, 192);
    aesManager.encryptPacket(warmupPacket, 256);

    //--------------------------------------------------
    // Fixed AES-128 (100 iterations, averaged)
    //--------------------------------------------------
    unsigned long startTime = micros();
    for(int i = 0; i < 100; i++)
    {
        result.fixed128 = aesManager.encryptPacket(packet, 128);
    }
    unsigned long endTime = micros();
    result.fixed128.encryptionTime_us = (float)(endTime - startTime) / 100.0;
    result.fixed128.estimatedEnergy_mJ = 
        3.3f * 0.08f * (result.fixed128.encryptionTime_us / 1000000.0f) * 1000.0f;

    //--------------------------------------------------
    // Fixed AES-192 (100 iterations, averaged)
    //--------------------------------------------------
    startTime = micros();
    for(int i = 0; i < 100; i++)
    {
        result.fixed192 = aesManager.encryptPacket(packet, 192);
    }
    endTime = micros();
    result.fixed192.encryptionTime_us = (float)(endTime - startTime) / 100.0;
    result.fixed192.estimatedEnergy_mJ = 
        3.3f * 0.08f * (result.fixed192.encryptionTime_us / 1000000.0f) * 1000.0f;

    //-------------------------AC-------------------------
    // Fixed AES-256 (100 iterations, averaged)
    //--------------------------------------------------
    startTime = micros();
    for(int i = 0; i < 100; i++)
    {
        result.fixed256 = aesManager.encryptPacket(packet, 256);
    }
    endTime = micros();
    result.fixed256.encryptionTime_us = (float)(endTime - startTime) / 100.0;
    result.fixed256.estimatedEnergy_mJ = 
        3.3f * 0.08f * (result.fixed256.encryptionTime_us / 1000000.0f) * 1000.0f;

    //--------------------------------------------------
    // Adaptive (single encryption + decision)
    //--------------------------------------------------
    float score = decisionEngine.calculateDecisionScore(packet);
    uint16_t mode = decisionEngine.selectAESMode(score);

    startTime = micros();
    result.adaptive = aesManager.encryptPacket(packet, mode);
    endTime = micros();

    result.adaptive.encryptionTime_us = (float)(endTime - startTime);
    result.adaptive.estimatedEnergy_mJ = 
        3.3f * 0.08f * (result.adaptive.encryptionTime_us / 1000000.0f) * 1000.0f;
    result.adaptive.decisionScore = score;

    return result;
}
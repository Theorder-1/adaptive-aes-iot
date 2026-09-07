#ifndef DECISION_REPORT_H
#define DECISION_REPORT_H

#include <Arduino.h>
#include "Packet.h"

// Define packet buffer size
// IoTPacket serialized = 43 bytes → padded to 48 bytes (3 AES blocks)
#define PACKET_BUFFER_SIZE 48

struct DecisionReport
{
    uint16_t selectedAES;

    float decisionScore;

    float encryptionTime_us;

    float estimatedEnergy_mJ;

    uint8_t ciphertext[PACKET_BUFFER_SIZE];
    uint8_t plaintext[PACKET_BUFFER_SIZE];

    uint8_t decryptedText[PACKET_BUFFER_SIZE];

    // IV used for CBC encryption — stored for decryption
    uint8_t iv[16];

    bool decryptionSuccess;

    // Reconstructed packet after decryption
    IoTPacket decryptedPacket;

    float batteryScore;
    float cpuScore;
    float priorityScore;
    float threatScore;

    float entropy;

    float avalancheEffect;

    float securityScore;
};

#endif
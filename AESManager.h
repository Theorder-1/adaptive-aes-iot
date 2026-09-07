#ifndef AES_MANAGER_H
#define AES_MANAGER_H

#include <Arduino.h>

// Include AES variant headers
#include "aes128.h"
#include "aes192.h"
#include "aes256.h"

#include "Packet.h"
#include "DecisionReport.h"

class AESManager
{
public:

    void begin();

    DecisionReport encryptPacket(
        const IoTPacket& packet,
        uint16_t aesMode
    );

    void decryptPacket(
        DecisionReport& report
    );

private:

    void serializePacket(
        const IoTPacket& packet,
        uint8_t buffer[PACKET_BUFFER_SIZE]
    );

    void deserializePacket(
        const uint8_t buffer[PACKET_BUFFER_SIZE],
        IoTPacket& packet
    );

    float estimateEnergy(float time_us);

    void printCipher(uint8_t cipher[], int length);

    const uint8_t key128[16] = {
        0x2B, 0x7E, 0x15, 0x16,
        0x28, 0xAE, 0xD2, 0xA6,
        0xAB, 0xF7, 0x15, 0x88,
        0x09, 0xCF, 0x4F, 0x3C
    };

    const uint8_t key192[24] = {
        0x8E, 0x73, 0xB0, 0xF7,
        0xDA, 0x0E, 0x64, 0x52,
        0xC8, 0x10, 0xF3, 0x2B,
        0x80, 0x90, 0x79, 0xE5,
        0x62, 0xF8, 0xEA, 0xD2,
        0x52, 0x2C, 0x6B, 0x7B
    };

    const uint8_t key256[32] = {
        0x60, 0x3D, 0xEB, 0x10,
        0x15, 0xCA, 0x71, 0xBE,
        0x2B, 0x73, 0xAE, 0xF0,
        0x85, 0x7D, 0x77, 0x81,
        0x1F, 0x35, 0x2C, 0x07,
        0x3B, 0x61, 0x08, 0xD7,
        0x2D, 0x98, 0x10, 0xA3,
        0x09, 0x14, 0xDF, 0xF4
    };
};

#endif
#include "AESManager.h"

void AESManager::begin()
{
    // Seed random generator for IV generation
    randomSeed(analogRead(0));
}

//------------------------------------------------------
// Serialize full IoTPacket into byte buffer
//------------------------------------------------------

void AESManager::serializePacket(
    const IoTPacket& packet,
    uint8_t buffer[PACKET_BUFFER_SIZE]
)
{
    memset(buffer, 0, PACKET_BUFFER_SIZE);
    int offset = 0;

    // packetID (4 bytes)
    memcpy(buffer + offset, &packet.packetID, 4);
    offset += 4;

    // timestamp (4 bytes)
    memcpy(buffer + offset, &packet.timestamp, 4);
    offset += 4;

    // temperature (4 bytes, float)
    memcpy(buffer + offset, &packet.temperature, 4);
    offset += 4;

    // humidity (4 bytes, float)
    memcpy(buffer + offset, &packet.humidity, 4);
    offset += 4;

    // pressure (4 bytes, float)
    memcpy(buffer + offset, &packet.pressure, 4);
    offset += 4;

    // gas (4 bytes, float)
    memcpy(buffer + offset, &packet.gas, 4);
    offset += 4;

    // accelX (4 bytes, float)
    memcpy(buffer + offset, &packet.accelX, 4);
    offset += 4;

    // accelY (4 bytes, float)
    memcpy(buffer + offset, &packet.accelY, 4);
    offset += 4;

    // accelZ (4 bytes, float)
    memcpy(buffer + offset, &packet.accelZ, 4);
    offset += 4;

    // battery (4 bytes, int)
    memcpy(buffer + offset, &packet.battery, 4);
    offset += 4;

    // cpuLoad (4 bytes, int)
    memcpy(buffer + offset, &packet.cpuLoad, 4);
    offset += 4;

    // priority (1 byte)
    buffer[offset] = packet.priority;
    offset += 1;

    // threatLevel (1 byte)
    buffer[offset] = packet.threatLevel;
    offset += 1;

    // aesMode (1 byte)
    buffer[offset] = packet.aesMode;
    offset += 1;
}

//------------------------------------------------------
// Deserialize byte buffer back to IoTPacket
//------------------------------------------------------

void AESManager::deserializePacket(
    const uint8_t buffer[PACKET_BUFFER_SIZE],
    IoTPacket& packet
)
{
    int offset = 0;

    memcpy(&packet.packetID, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.timestamp, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.temperature, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.humidity, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.pressure, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.gas, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.accelX, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.accelY, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.accelZ, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.battery, buffer + offset, 4);
    offset += 4;

    memcpy(&packet.cpuLoad, buffer + offset, 4);
    offset += 4;

    packet.priority = buffer[offset];
    offset += 1;

    packet.threatLevel = buffer[offset];
    offset += 1;

    packet.aesMode = buffer[offset];
    offset += 1;
}

//------------------------------------------------------
// Energy estimation
//------------------------------------------------------

float AESManager::estimateEnergy(float time_us)
{
    float voltage = 3.3f;
    float current = 0.08f;
    float seconds = time_us / 1000000.0f;

    return voltage * current * seconds * 1000.0f;
}

//------------------------------------------------------
// Print cipher bytes
//------------------------------------------------------

void AESManager::printCipher(uint8_t cipher[], int length)
{
    for (int i = 0; i < length; i++)
    {
        if (cipher[i] < 16)
            Serial.print("0");

        Serial.print(cipher[i], HEX);
        Serial.print(" ");
    }

    Serial.println();
}

//------------------------------------------------------
// Encrypt Packet using selected AES mode
//------------------------------------------------------

DecisionReport AESManager::encryptPacket(
    const IoTPacket& packet,
    uint16_t aesMode
)
{
    DecisionReport report;

    report.selectedAES = aesMode;
    report.decryptionSuccess = false;

    // Serialize full packet into byte buffer
    serializePacket(packet, report.plaintext);

    // Copy plaintext into ciphertext buffer
    memcpy(report.ciphertext, report.plaintext, PACKET_BUFFER_SIZE);

    //--------------------------------------------------
    // Generate RANDOM IV and store in report
    //--------------------------------------------------
    for(int i = 0; i < 16; i++)
    {
        report.iv[i] = (uint8_t)random(0, 256);
    }

    unsigned long startTime = micros();

    switch (aesMode)
    {
        case 128:
        {
            struct AES128_ctx ctx;
            AES128_init_ctx_iv(&ctx, key128, report.iv);
            AES128_CBC_encrypt_buffer(&ctx, report.ciphertext, PACKET_BUFFER_SIZE);
            break;
        }

        case 192:
        {
            struct AES192_ctx ctx;
            AES192_init_ctx_iv(&ctx, key192, report.iv);
            AES192_CBC_encrypt_buffer(&ctx, report.ciphertext, PACKET_BUFFER_SIZE);
            break;
        }

        case 256:
        {
            struct AES256_ctx ctx;
            AES256_init_ctx_iv(&ctx, key256, report.iv);
            AES256_CBC_encrypt_buffer(&ctx, report.ciphertext, PACKET_BUFFER_SIZE);
            break;
        }

        default:
            report.selectedAES = 128;

            struct AES128_ctx ctx;
            AES128_init_ctx_iv(&ctx, key128, report.iv);
            AES128_CBC_encrypt_buffer(&ctx, report.ciphertext, PACKET_BUFFER_SIZE);
            break;
    }

    unsigned long endTime = micros();

    report.encryptionTime_us = (float)(endTime - startTime);
    report.estimatedEnergy_mJ = estimateEnergy(report.encryptionTime_us);

    return report;
}

//------------------------------------------------------
// Decrypt Packet
//------------------------------------------------------

void AESManager::decryptPacket(DecisionReport& report)
{
    // Copy ciphertext into decryption buffer
    memcpy(report.decryptedText, report.ciphertext, PACKET_BUFFER_SIZE);

    // Use the SAME IV that was stored during encryption
    switch (report.selectedAES)
    {
        case 128:
        {
            struct AES128_ctx ctx;
            AES128_init_ctx_iv(&ctx, key128, report.iv);
            AES128_CBC_decrypt_buffer(&ctx, report.decryptedText, PACKET_BUFFER_SIZE);
            break;
        }

        case 192:
        {
            struct AES192_ctx ctx;
            AES192_init_ctx_iv(&ctx, key192, report.iv);
            AES192_CBC_decrypt_buffer(&ctx, report.decryptedText, PACKET_BUFFER_SIZE);
            break;
        }

        case 256:
        {
            struct AES256_ctx ctx;
            AES256_init_ctx_iv(&ctx, key256, report.iv);
            AES256_CBC_decrypt_buffer(&ctx, report.decryptedText, PACKET_BUFFER_SIZE);
            break;
        }

        default:
            report.decryptionSuccess = false;
            return;
    }

    // Deserialize decrypted bytes back to IoTPacket
    deserializePacket(report.decryptedText, report.decryptedPacket);

    // Compare decrypted data with original plaintext
    report.decryptionSuccess = true;

    for (int i = 0; i < PACKET_BUFFER_SIZE; i++)
    {
        if (report.decryptedText[i] != report.plaintext[i])
        {
            report.decryptionSuccess = false;
            break;
        }
    }
}
#include "CSVLogger.h"

void CSVLogger::begin()
{
    printHeader();
}

void CSVLogger::printHeader()
{
    Serial.println();
    Serial.println("============= CSV DATA =============");

    Serial.println(
    "PacketID,"
    "Timestamp,"
    "Battery,"
    "CPU,"
    "Priority,"
    "Threat,"
    "DecisionScore,"
    "AESMode,"
    "EncryptionTime_us,"
    "Energy_mJ");
}

void CSVLogger::log(const IoTPacket &packet,
                    const DecisionReport &report,
                    float decisionScore)
{
    Serial.print(packet.packetID);
    Serial.print(",");

    Serial.print(packet.timestamp);
    Serial.print(",");

    Serial.print(packet.battery);
    Serial.print(",");

    Serial.print(packet.cpuLoad);
    Serial.print(",");

    Serial.print(packet.priority);
    Serial.print(",");

    Serial.print(packet.threatLevel);
    Serial.print(",");

    Serial.print(decisionScore,2);
    Serial.print(",");

    Serial.print(report.selectedAES);
    Serial.print(",");

    Serial.print(report.encryptionTime_us);
    Serial.print(",");

    Serial.println(report.estimatedEnergy_mJ,6);
}
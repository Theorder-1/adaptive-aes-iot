#include "ResultAnalyzer.h"

void ResultAnalyzer::begin()
{
    packetCount = 0;

    aes128Count = 0;
    aes192Count = 0;
    aes256Count = 0;

    totalDecisionScore = 0;
    totalEncryptionTime = 0;
    totalEnergy = 0;

    totalEntropy = 0;
    totalAvalanche = 0;
    totalSecurityScore = 0;
}

void ResultAnalyzer::update(const DecisionReport &report, float decisionScore)
{
    packetCount++;

    totalDecisionScore += decisionScore;
    totalEncryptionTime += report.encryptionTime_us;
    totalEnergy += report.estimatedEnergy_mJ;

    totalEntropy += report.entropy;
    totalAvalanche += report.avalancheEffect;
    totalSecurityScore += report.securityScore;

    switch(report.selectedAES)
    {
        case 128:
            aes128Count++;
            break;

        case 192:
            aes192Count++;
            break;

        case 256:
            aes256Count++;
            break;
    }
}

void ResultAnalyzer::printSummary()
{
    if(packetCount == 0)
        return;

    Serial.println();
    Serial.println("============== EXPERIMENT SUMMARY ==============");

    Serial.print("Packets Processed      : ");
    Serial.println(packetCount);

    Serial.print("AES-128 Selected       : ");
    Serial.println(aes128Count);

    Serial.print("AES-192 Selected       : ");
    Serial.println(aes192Count);

    Serial.print("AES-256 Selected       : ");
    Serial.println(aes256Count);

    Serial.println();

    Serial.print("Average Decision Score : ");
    Serial.println(totalDecisionScore / packetCount,2);

    Serial.print("Average Time (us)      : ");
    Serial.println(totalEncryptionTime / packetCount,2);

    Serial.print("Average Energy (mJ)    : ");
    Serial.println(totalEnergy / packetCount,6);

    Serial.print("Average Entropy        : ");
    Serial.println(totalEntropy / packetCount,2);

    Serial.print("Average Avalanche (%)  : ");
    Serial.println(totalAvalanche / packetCount,2);

    Serial.print("Average Security Score : ");
    Serial.println(totalSecurityScore / packetCount,2);

    Serial.println("===============================================");
}
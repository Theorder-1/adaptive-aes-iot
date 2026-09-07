#ifndef RESULT_ANALYZER_H
#define RESULT_ANALYZER_H

#include <Arduino.h>
#include "DecisionReport.h"

class ResultAnalyzer
{
public:

    void begin();

    void update(const DecisionReport &report, float decisionScore);

    void printSummary();

private:

    uint32_t packetCount;

    uint32_t aes128Count;
    uint32_t aes192Count;
    uint32_t aes256Count;

    float totalDecisionScore;
    float totalEncryptionTime;
    float totalEnergy;

    float totalEntropy;
    float totalAvalanche;
    float totalSecurityScore;
};

#endif
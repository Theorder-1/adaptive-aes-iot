#ifndef SECURITY_ANALYZER_H
#define SECURITY_ANALYZER_H

#include <Arduino.h>
#include "DecisionReport.h"

class SecurityAnalyzer
{
public:

    void begin();

    float calculateAvalanche(const uint8_t *plain,
                             const uint8_t *cipher,
                             int size);

    float calculateEntropy(const uint8_t *data,
                           int size);

    float calculateSecurityScore(float avalanche,
                                 float entropy);

    void analyze(const uint8_t *plain,
                 DecisionReport &report);

private:

    // Ciphertext accumulation buffer for entropy calculation
    static const int ENTROPY_BUFFER_SIZE = 256;
    uint8_t ciphertextBuffer[ENTROPY_BUFFER_SIZE];
    int ciphertextIndex = 0;
    bool bufferFull = false;

    void accumulateCiphertext(const uint8_t *ciphertext, int size);

    float calculateAccumulatedEntropy();

};

#endif
#include "SecurityAnalyzer.h"
#include <math.h>

void SecurityAnalyzer::begin()
{
    ciphertextIndex = 0;
    bufferFull = false;
}

//------------------------------------------------------
// Accumulate Ciphertext
//------------------------------------------------------
// Collects ciphertext bytes from multiple packets
// until buffer is full (256 bytes).
//------------------------------------------------------

void SecurityAnalyzer::accumulateCiphertext(
    const uint8_t *ciphertext,
    int size
)
{
    for(int i = 0; i < size; i++)
    {
        if(ciphertextIndex < ENTROPY_BUFFER_SIZE)
        {
            ciphertextBuffer[ciphertextIndex] = ciphertext[i];
            ciphertextIndex++;
        }
        else
        {
            bufferFull = true;
            return;
        }
    }
}

//------------------------------------------------------
// Calculate Entropy on Accumulated Data
//------------------------------------------------------
// Once buffer is full, calculates Shannon entropy
// on all 256 bytes. This gives meaningful values
// approaching 8.0 bits/byte.
//------------------------------------------------------

float SecurityAnalyzer::calculateAccumulatedEntropy()
{
    if(ciphertextIndex == 0)
        return 0.0f;

    int frequency[256];

    for(int i = 0; i < 256; i++)
        frequency[i] = 0;

    for(int i = 0; i < ciphertextIndex; i++)
        frequency[ciphertextBuffer[i]]++;

    float entropy = 0.0f;

    for(int i = 0; i < 256; i++)
    {
        if(frequency[i] == 0)
            continue;

        float p = (float)frequency[i] / ciphertextIndex;

        entropy -= p * (log(p) / log(2.0));
    }

    return entropy;
}

//------------------------------------------------------
// Avalanche Effect (Single Packet)
//------------------------------------------------------

float SecurityAnalyzer::calculateAvalanche(const uint8_t *plain,
                                           const uint8_t *cipher,
                                           int size)
{
    int changedBits = 0;
    int totalBits = size * 8;

    for(int i = 0; i < size; i++)
    {
        uint8_t diff = plain[i] ^ cipher[i];

        for(int b = 0; b < 8; b++)
        {
            if(diff & (1 << b))
                changedBits++;
        }
    }

    return (changedBits * 100.0) / totalBits;
}

//------------------------------------------------------
// Entropy (Single Packet — kept for reference)
//------------------------------------------------------

float SecurityAnalyzer::calculateEntropy(const uint8_t *data,
                                         int size)
{
    int frequency[256];

    for(int i = 0; i < 256; i++)
        frequency[i] = 0;

    for(int i = 0; i < size; i++)
        frequency[data[i]]++;

    float entropy = 0.0f;

    for(int i = 0; i < 256; i++)
    {
        if(frequency[i] == 0)
            continue;

        float p = (float)frequency[i] / size;

        entropy -= p * (log(p) / log(2.0));
    }

    return entropy;
}

//------------------------------------------------------
// Security Score
//------------------------------------------------------

float SecurityAnalyzer::calculateSecurityScore(float avalanche,
                                               float entropy)
{
    float score =
        (avalanche * 0.7f) +
        (entropy * 10.0f * 0.3f);

    if(score > 100.0f)
        score = 100.0f;

    return score;
}

//------------------------------------------------------
// Analyze
//------------------------------------------------------
// Calculates avalanche on single packet.
// Accumulates ciphertext for entropy.
// Stores results in DecisionReport.
//------------------------------------------------------

void SecurityAnalyzer::analyze(const uint8_t *plain,
                               DecisionReport &report)
{
    // Accumulate ciphertext for entropy calculation
    accumulateCiphertext(
        report.ciphertext,
        PACKET_BUFFER_SIZE
    );

    // Avalanche: single packet (plaintext vs ciphertext)
    float avalanche =
        calculateAvalanche(plain,
                           report.ciphertext,
                           PACKET_BUFFER_SIZE);

    // Entropy: accumulated ciphertext (256 bytes) if available
    float entropy;

    if(bufferFull)
    {
        entropy = calculateAccumulatedEntropy();
    }
    else
    {
        // Not enough data yet — use single packet entropy
        // (but mark it as partial)
        entropy = calculateEntropy(
            report.ciphertext,
            PACKET_BUFFER_SIZE
        );
    }

    float security =
        calculateSecurityScore(avalanche,
                               entropy);

    // STORE results in report
    report.avalancheEffect = avalanche;
    report.entropy = entropy;
    report.securityScore = security;

    // Print to Serial
    Serial.println();
    Serial.println("========= SECURITY =========");

    Serial.print("Avalanche Effect (%) : ");
    Serial.println(avalanche, 2);

    Serial.print("Entropy (bits/byte)  : ");
    Serial.println(entropy, 4);

    if(!bufferFull)
    {
        Serial.print("  [Accumulating: ");
        Serial.print(ciphertextIndex);
        Serial.print("/");
        Serial.print(ENTROPY_BUFFER_SIZE);
        Serial.println(" bytes]");
    }
    else
    {
        Serial.println("  [Full 256-byte sample]");
    }

    Serial.print("Security Score       : ");
    Serial.println(security, 2);

    Serial.println("============================");
}
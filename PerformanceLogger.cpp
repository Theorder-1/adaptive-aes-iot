#include "PerformanceLogger.h"

void PerformanceLogger::begin()
{

}

//------------------------------------------------------
// Throughput
//------------------------------------------------------
// Calculates bytes per second.
// Uses PACKET_BUFFER_SIZE (48 bytes) instead of 16.
//------------------------------------------------------

float PerformanceLogger::calculateThroughput(float time_us)
{
    if(time_us <= 0)
        return 0;

    return (PACKET_BUFFER_SIZE * 1000000.0) / time_us;
}

//------------------------------------------------------
// Efficiency
//------------------------------------------------------
// Calculates bytes per millijoule.
// Uses PACKET_BUFFER_SIZE (48 bytes) instead of 16.
//------------------------------------------------------

float PerformanceLogger::calculateEfficiency(float energy_mJ)
{
    if(energy_mJ <= 0)
        return 0;

    return PACKET_BUFFER_SIZE / energy_mJ;
}

//------------------------------------------------------
// Power
//------------------------------------------------------
// Calculates estimated power draw in watts.
//------------------------------------------------------

float PerformanceLogger::calculatePower(float energy_mJ, float time_us)
{
    if(time_us <= 0)
        return 0;

    float time_sec = time_us / 1000000.0;

    return (energy_mJ / 1000.0) / time_sec;
}

//------------------------------------------------------
// Log
//------------------------------------------------------
// Displays performance metrics for a DecisionReport.
//------------------------------------------------------

void PerformanceLogger::log(const DecisionReport &report)
{
    Serial.println();
    Serial.println("========== PERFORMANCE ==========");

    Serial.print("AES Mode             : AES-");
    Serial.println(report.selectedAES);

    Serial.print("Packet Size (bytes)  : ");
    Serial.println(PACKET_BUFFER_SIZE);

    Serial.print("Encryption Time (us) : ");
    Serial.println(report.encryptionTime_us);

    Serial.print("Energy (mJ)          : ");
    Serial.println(report.estimatedEnergy_mJ, 6);

    Serial.print("Throughput (B/s)     : ");
    Serial.println(calculateThroughput(report.encryptionTime_us), 2);

    Serial.print("Efficiency (B/mJ)    : ");
    Serial.println(calculateEfficiency(report.estimatedEnergy_mJ), 2);

    Serial.print("Estimated Power (W)  : ");
    Serial.println(calculatePower(report.estimatedEnergy_mJ,
                                  report.encryptionTime_us), 4);

    Serial.println("=================================");
}
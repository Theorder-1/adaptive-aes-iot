#ifndef PERFORMANCE_LOGGER_H
#define PERFORMANCE_LOGGER_H

#include <Arduino.h>
#include "DecisionReport.h"

class PerformanceLogger
{
public:

    void begin();

    void log(const DecisionReport &report);

    float calculateThroughput(float time_us);

    float calculateEfficiency(float energy_mJ);

    float calculatePower(float energy_mJ, float time_us);

};

#endif
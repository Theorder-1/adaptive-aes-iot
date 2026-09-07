#ifndef COMPARATOR_H
#define COMPARATOR_H

#include <Arduino.h>

#include "Packet.h"
#include "DecisionReport.h"
#include "DecisionEngine.h"
#include "AESManager.h"

struct ComparisonReport
{
    DecisionReport fixed128;

    DecisionReport fixed192;

    DecisionReport fixed256;

    DecisionReport adaptive;
};

class Comparator
{
public:

    void begin();

    ComparisonReport compare(IoTPacket &packet);

private:

    DecisionEngine decisionEngine;

    AESManager aesManager;
};

#endif
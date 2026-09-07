#ifndef CSV_LOGGER_H
#define CSV_LOGGER_H

#include <Arduino.h>
#include "Packet.h"
#include "DecisionReport.h"

class CSVLogger
{
public:

    void begin();

    void printHeader();

    void log(const IoTPacket &packet,
             const DecisionReport &report,
             float decisionScore);

};

#endif
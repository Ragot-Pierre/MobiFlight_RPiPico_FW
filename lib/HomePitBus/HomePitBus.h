#pragma once

#include <inttypes.h>
#include <Arduino.h>

class HomePitBus
{
private:
    Stream *serialPort;
public:
    HomePitBus(Stream &comms);
    ~HomePitBus();


};

HomePitBus::HomePitBus(Stream &comms)
{
}

HomePitBus::~HomePitBus()
{
}

#pragma once

#include <inttypes.h>
#include <Arduino.h>

class HomePitBus
{
private:
    SerialUART *serialPort;

public:
    HomePitBus(SerialUART &comms);

    void Init(uint32_t baudrate = 460800);
    void Send(uint8_t cmd, uint8_t addr, uint8_t data, uint8_t value);
    void ProcessData();



    uint8_t FIFO[64][3];
    uint8_t index = 0;
};
#include <HomePitBus.hpp>

HomePitBus::HomePitBus(SerialUART &comms)
{
    serialPort = &comms;
}

void HomePitBus::Init(uint baudrate)
{
    if (serialPort == &Serial2)
    {
        serialPort->setTX(4);
        serialPort->setRX(5);
    }    

    serialPort->begin(baudrate);
}

void HomePitBus::Send(uint8_t cmd, uint8_t addr, uint8_t data, uint8_t value)
{
    serialPort->write(cmd);
    serialPort->write(addr);
    serialPort->write(data);
    serialPort->write(value);
}

void HomePitBus::ProcessData()
{
    while (serialPort->available())
    {
        serialPort->readBytes(FIFO[index], 3);
        index++;
    }
}

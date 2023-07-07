#pragma once
#include "FlashMemoryDevice.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);
    void checkReadValidation(int readResult[5]);
protected:
    FlashMemoryDevice* m_hardware;
    void read5times(long address, int readResult[5]);
};
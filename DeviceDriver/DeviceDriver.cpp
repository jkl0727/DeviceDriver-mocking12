#include <windows.h>
#include <stdexcept>
#include "DeviceDriver.h"
using namespace std;

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int readResult[5] = { 0, };
    for (int i = 0; i < 5; ++i)
    {
        readResult[i] = m_hardware->read(address);
    	Sleep(200);
    }

    for (int i = 1; i < 5; ++i)
    {
	    if (readResult[0] != readResult[i])
	    {
            throw logic_error("values are wrong");
	    }
    }

    return (int)(m_hardware->read(address));
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}
#include <windows.h>
#include <stdexcept>
#include "DeviceDriver.h"
using namespace std;

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

void DeviceDriver::read5times(long address, int readResult[5])
{
	for (int i = 0; i < 5; ++i)
	{
		readResult[i] = m_hardware->read(address);
		Sleep(200);
	}
}

void DeviceDriver::checkReadValidation(int readResult[5])
{
	for (int i = 1; i < 5; ++i)
	{
		if (readResult[0] != readResult[i])
		{
			throw logic_error("values are wrong");
		}
	}
}

int DeviceDriver::read(long address)
{
    int readResult[5] = { 0, };
    read5times(address, readResult);
    checkReadValidation(readResult);

    return (int)(readResult[0]);
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}
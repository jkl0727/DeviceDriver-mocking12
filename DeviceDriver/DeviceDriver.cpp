#include <windows.h>
#include <stdexcept>
#include <exception>

#include "DeviceDriver.h"
using namespace std;

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

class WriteFailException : public exception {
public:
	void writeFail()
	{
		throw exception("already written");
	}
};

void DeviceDriver::read5times(long address, int readResult[5])
{
	for (int i = 0; i < 5; ++i)
	{
		readResult[i] = (int)m_hardware->read(address);
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
    if (m_hardware->read(address) == 0xFF)
    {
		m_hardware->write(address, (unsigned char)data);
    }
	else
	{
		WriteFailException e;
		e.writeFail();
	}
}
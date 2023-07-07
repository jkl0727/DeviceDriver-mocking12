#include <vector>
#include "DeviceDriver.h"
using namespace std;

class Application
{
public:
	Application(FlashMemoryDevice* hardware) : m_hardware(hardware){}
	vector<int> ReadAndPrint(int startAddr, int endAddr)
	{
		vector<int> result;
		for(int i = startAddr; i < endAddr; ++i)
		{
			result.push_back(m_hardware->read(i));
		}
		return result;
	}

	void WriteAll(int value)
	{
		for(int i = 0; i  < 5; ++i)
		{
			m_hardware->write(i, value);
		}
	}
private:
	FlashMemoryDevice* m_hardware;
};
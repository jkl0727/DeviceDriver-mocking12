#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../DeviceDriver/DeviceDriver.cpp"
using namespace testing;

class FlashMemoryDriverMock : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long address), ());
	MOCK_METHOD(void, write, (long address, unsigned char data), ());
};

TEST(DeviceDriver, read) {

	FlashMemoryDriverMock hwMock;
	EXPECT_CALL(hwMock, read(0x100))
		.WillRepeatedly(Return(0xAB));

	DeviceDriver driver{ &hwMock };

	EXPECT_EQ(driver.read(0x100), 0xAB);
}
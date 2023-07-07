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
		.Times(5)
		.WillRepeatedly(Return(0xAB));

	DeviceDriver driver{ &hwMock };

	EXPECT_EQ(driver.read(0x100), 0xAB);
}

TEST(DeviceDriver, readException) {

	FlashMemoryDriverMock hwMock;
	EXPECT_CALL(hwMock, read(0x100))
		.Times(5)
		.WillOnce(Return(0xAB))
		.WillOnce(Return(0xAB))
		.WillRepeatedly(Return(0xBB));

	DeviceDriver driver{ &hwMock };

	EXPECT_THROW(driver.read(0x100), logic_error);
}

TEST(DeviceDriver, write) {

	FlashMemoryDriverMock hwMock;
	EXPECT_CALL(hwMock, read(0x100))
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(hwMock, write(0x100, 0xAA));

	DeviceDriver driver{ &hwMock };

	driver.write(0x100, 0xAA);

	EXPECT_CALL(hwMock, read(0x100))
		.WillRepeatedly(Return(0xAA));
	EXPECT_EQ(driver.read(0x100), 0xAA);
}

TEST(DeviceDriver, writeExpection) {

	FlashMemoryDriverMock hwMock;
	EXPECT_CALL(hwMock, read(0x100))
		.WillRepeatedly(Return(0xAA));
	EXPECT_CALL(hwMock, write(0x100, 0xAA))
		.Times(0);

	DeviceDriver driver{ &hwMock };

	EXPECT_ANY_THROW(driver.write(0x100, 0xAA));
}
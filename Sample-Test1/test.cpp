#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../DeviceDriver/DeviceDriver.cpp"
#include "../DeviceDriver/App.cpp"
using namespace testing;

#define TEST_ADDRESS 0x100

class FlashMemoryDriverMock : public FlashMemoryDevice
{
public:
	MOCK_METHOD(unsigned char, read, (long address), ());
	MOCK_METHOD(void, write, (long address, unsigned char data), ());
};

TEST(DeviceDriver, read) {

	FlashMemoryDriverMock hwMock;
	EXPECT_CALL(hwMock, read(TEST_ADDRESS))
		.Times(5)
		.WillRepeatedly(Return(0xAB));

	DeviceDriver driver{ &hwMock };

	EXPECT_EQ(driver.read(TEST_ADDRESS), 0xAB);
}

TEST(DeviceDriver, readException) {

	FlashMemoryDriverMock hwMock;
	EXPECT_CALL(hwMock, read(TEST_ADDRESS))
		.Times(5)
		.WillOnce(Return(0xAB))
		.WillOnce(Return(0xAB))
		.WillRepeatedly(Return(0xBB));

	DeviceDriver driver{ &hwMock };

	EXPECT_THROW(driver.read(TEST_ADDRESS), logic_error);
}

TEST(DeviceDriver, write) {

	FlashMemoryDriverMock hwMock;
	EXPECT_CALL(hwMock, read(TEST_ADDRESS))
		.WillRepeatedly(Return(0xFF));
	EXPECT_CALL(hwMock, write(TEST_ADDRESS, 0xAA));

	DeviceDriver driver{ &hwMock };

	driver.write(TEST_ADDRESS, 0xAA);

	EXPECT_CALL(hwMock, read(TEST_ADDRESS))
		.WillRepeatedly(Return(0xAA));
	EXPECT_EQ(driver.read(TEST_ADDRESS), 0xAA);
}

TEST(DeviceDriver, writeExpection) {

	FlashMemoryDriverMock hwMock;
	EXPECT_CALL(hwMock, read(TEST_ADDRESS))
		.WillRepeatedly(Return(0xAA));
	EXPECT_CALL(hwMock, write(TEST_ADDRESS, 0xAA))
		.Times(0);

	DeviceDriver driver{ &hwMock };

	EXPECT_ANY_THROW(driver.write(TEST_ADDRESS, 0xAA));
}

class ApplicationFixture : public testing::Test
{
public:
	FlashMemoryDriverMock hwMock;
	Application app{ &hwMock };
	vector<int> result;
};

TEST_F(ApplicationFixture, readAndPrint) {

	for (int i = 0; i < 10; ++i)
	{
		EXPECT_CALL(hwMock, read(i))
			.WillRepeatedly(Return(i));
	}

	result = app.ReadAndPrint(0, 10);

	for (int i = 0; i < 10; ++i)
	{
		cout << result[i] << endl;
		EXPECT_EQ(result[i], i);
	}
}

TEST_F(ApplicationFixture, writeAll) {

	int value = 0xAA;
	for (int i = 0; i < 5; ++i)
	{
		EXPECT_CALL(hwMock, read(i))
			.WillRepeatedly(Return(0xFF));
		EXPECT_CALL(hwMock, write(i, value));
	}
	app.WriteAll(value);

	for (int i = 0; i < 5; ++i)
	{
		EXPECT_CALL(hwMock, read(i))
			.WillRepeatedly(Return(value));
	}
	result = app.ReadAndPrint(0, 5);

	for (int i = 0; i < 5; ++i)
	{
		cout << result[i] << endl;
		EXPECT_EQ(result[i], value);
	}
}
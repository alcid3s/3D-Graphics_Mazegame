#include "pch.h"
#include "../FileEditor.h"
#include "../FileEditor.cpp"

constexpr int character = 48;

class FileIOTest : public ::testing::Test {
public:
	FileIOTest();
	~FileIOTest();

	void SetUp() override;

	void writeDataToFile(const int& duration = 4);
	std::vector<int> readData();
protected:
	const std::string fileName = "fakestats.txt";
	FileEditor* fileIO;
	clock_t timeRunning = 0;

};
FileIOTest::FileIOTest()
{
}
FileIOTest::~FileIOTest()
{
	delete fileIO;
}
void FileIOTest::writeDataToFile(const int& duration) {
	if (duration > 9 || 1 > duration)
		throw "duration must be between 1 and 9";

	for (int i = 0; i < 5; i++) {

		// must wait until the time has passed because FileEditor also works with clock();
		while (clock() / CLOCKS_PER_SEC <= duration);
		fileIO->writeStats(true);
	}
}
void FileIOTest::SetUp()
{
	this->fileIO = new FileEditor(fileName);
}
std::vector<int> FileIOTest::readData() {
	std::string text = fileIO->getStats(false);

	std::vector<int> numbers;

	for (auto& c : text) {
		if (isdigit(c)) {
			numbers.push_back(c);
		}
	}
	return numbers;
}

TEST_F(FileIOTest, testFileIO) {
	writeDataToFile(4);
	std::vector<int> values = readData();

	EXPECT_FALSE(values.empty());

	for (int i = 0; i < values.size(); i++) {
		if (i % 2 == 1) {
			EXPECT_EQ((values[i] - character <= 9 && values[i] - character >= 1), 1);
		}
	}
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
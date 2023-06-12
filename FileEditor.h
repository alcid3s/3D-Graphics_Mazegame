#pragma once
#include <string>
#include <ctime>

class FileEditor {
public:
	FileEditor(const std::string& fileName);
	~FileEditor();

	void begin();
	void writeStats(bool verbose);
	std::string getStats(bool verbose);
private:
	int round = 1;
	std::string fileName;
	clock_t applicationRunning = 0;
};
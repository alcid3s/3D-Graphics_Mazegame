#include "FileEditor.h"
#include <iostream>
#include <fstream>


FileEditor::FileEditor(const std::string& fileName) : fileName(fileName)
{

}

FileEditor::~FileEditor()
{
}

void FileEditor::begin() {
	static bool started = false;
	if (!started) {
		started = true;
		this->applicationRunning = clock();
	}
}

void FileEditor::writeStats() {
	std::ofstream file;
	file.open(fileName, std::ios::app);
	file << "level: " << round << " finished in " << (clock() - this->applicationRunning) / CLOCKS_PER_SEC << " seconds\n";
	this->round++;
	file.close();
}

void FileEditor::getStats() {
	std::string line;
	std::ifstream file(fileName);
	if (file.is_open()) {
		std::cout << "stats of last game:\n";
		while (std::getline(file, line)) {
			std::cout << line << "\n";
		}
		file.close();
		std::remove(fileName.c_str());
	}
}
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

void FileEditor::writeStats(bool verbose) {
	std::ofstream file;
	file.open(fileName, std::ios::app);
	file << "level: " << round << " finished in " << (clock() - this->applicationRunning) / CLOCKS_PER_SEC << " seconds\n";
	if (verbose)
		std::cout << "level: " << round << " finished in " << (clock() - this->applicationRunning) / CLOCKS_PER_SEC << " seconds\n";
	this->round++;
	file.close();
}

std::string FileEditor::getStats(bool verbose) {
	std::string fullText;
	std::string line;
	std::ifstream file(fileName);
	if (file.is_open()) {

		if (verbose)
			std::cout << "stats of last game:\n";
		while (std::getline(file, line)) {
			if (verbose)
				std::cout << line << "\n";
			fullText += line;
			fullText += "\n";
		}
		file.close();

		if (verbose)
			std::remove(fileName.c_str());
	}

	return fullText;
}
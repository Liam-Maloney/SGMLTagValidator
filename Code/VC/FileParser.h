#pragma once
#include <fstream>

class FileParser
{
	std::fstream fileHandle;
	bool hasMoreToParse();
public:
	std::string getContent();
	FileParser(std::string fileToParseFrom);
	~FileParser();
};
#pragma once
#include <fstream>
#include "IO.h"

class FileParser : public IO
{
	std::fstream fileHandle;
	bool hasMoreToParse();
public:
	std::string getContent();
	FileParser(std::string fileToParseFrom);
	~FileParser();
};
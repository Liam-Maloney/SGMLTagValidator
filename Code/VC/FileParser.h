#pragma once
#include <fstream>
#include "IO.h"

class FileParser : public IO
{
	std::fstream fileHandle;
	bool hasMoreToParse();
	std::string readFile();
	std::vector<int> findOpenComments(std::string fullContent);
	std::vector<int> findClosingComments(std::string fullContent);
	std::string removeCommentedSectionsFromSource(
		std::vector<int> openCommentLocations, std::vector<int> closeCommentLocations,
		std::string fullContent);
public:
	std::string getContent();
	FileParser(std::string fileToParseFrom);
};
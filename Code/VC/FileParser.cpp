#include "stdafx.h"
#include "FileParser.h"

FileParser::FileParser(std::string fileToParseFrom)
{
	fileHandle.open(fileToParseFrom);
	if (!(fileHandle.is_open()))
	{
		throw std::runtime_error("Error Opening File Stream to " + fileToParseFrom + "\n");
	}
}

bool FileParser::hasMoreToParse()
{
	return (fileHandle.eof()) ? false : true;
}

std::string FileParser::getContent()
{
	std::string fullContent = "";
	char nextChar;
	
	fileHandle.get(nextChar);
	while (hasMoreToParse())
	{
		fullContent += nextChar;
		fileHandle.get(nextChar);
	}
	return fullContent;
}

FileParser::~FileParser()
{
	fileHandle.close();
}
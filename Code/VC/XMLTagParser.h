#pragma once
#include "TagParser.h"
#include "XMLTag.h"
#include "FileParser.h"
#include <string>
#include <list>
#include <fstream>
#include "FileParser.h"

class XMLTagParser : public TagParser 
{
	int lineNumber = 0;
	std::list<Tag> nextTagsInFile(std::string parseTagsFrom);
	bool XMLTagParser::notAtEndOfInputFile();
	void openFileHandleFor(std::string fileToParseFrom);
public:
	std::list<Tag> getTagsAsListParsedFrom(FileParser& file);
	
	std::list<Tag> parseTagsFrom(std::string fileToParseFrom);
	
	XMLTagParser();
	XMLTagParser(std::string testForNow);
	~XMLTagParser();
};
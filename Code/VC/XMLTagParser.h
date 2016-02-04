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
public:struct simpleTag;
private:
	std::list<Tag*> parseFromSource(std::string source);
	void openFileHandleFor(std::string fileToParseFrom);
	std::list<Tag*> formTagsAsObjects(std::list<XMLTagParser::simpleTag> fullTags);
	bool notAtEndOfName(char endOfTagDelimiter);
	std::list<std::string> readTagAttributes(std::string currentTag);
	std::string readTagFullName(std::string fullTag);

public:
	
	std::list<Tag*> getTagsAsListParsedFrom(FileParser& file);
	std::list<Tag*> parseTagsFrom(std::string fileToParseFrom);
	
	XMLTagParser();
	XMLTagParser(std::string testForNow);
	~XMLTagParser();
};
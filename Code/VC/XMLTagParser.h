#pragma once
#include "TagParser.h"
#include <string>
#include <list>
#include <fstream>
#include "TagFactory.h"
#include "IOFactory.h"

class XMLTagParser : public TagParser 
{
public:struct simpleTag;
private:

	TagFactory tagSupplier;
	IOFactory ioHandle;

	std::list<std::string> finallyGetAttributes(std::string processedAttr);
	std::string removeSpacesAroundEquals(std::string tagWithRemovedSpaces);
	std::string removeExtraSpaces(std::string currentTag);
	std::list<Tag*> parseFromSource(std::string source);
	void openFileHandleFor(std::string fileToParseFrom);
	std::list<Tag*> formTagsAsObjects(std::list<XMLTagParser::simpleTag> fullTags);
	bool notAtEndOfName(char endOfTagDelimiter);
	std::list<std::string> readTagAttributes(std::string currentTag);
	std::string readTagFullName(std::string fullTag);
	Tag* getNewTagWithAttributesSet(std::string tagName, std::list<std::string> attributes, int lineNumber);

public:
	
	std::list<Tag*> getTagsAsListParsedFrom(IO* inputSource);
	std::list<Tag*> parseTagsFrom(std::string fileToParseFrom);
	
	XMLTagParser();
	XMLTagParser(std::string testForNow);
	~XMLTagParser();
};
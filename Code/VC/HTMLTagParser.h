#pragma once
#include "SGMLParser.h"
#include "TagParserInterface.h"
#include <map>

class HTMLTagParser : public TagParserInterface, public SGMLParser
{
	std::map<std::string, bool> voidTags;
	Tag* getTagAsObject(Lexer::tokenLineNumPairs token); //
	std::vector<Tag*> formTagsAsObjects(std::vector<Lexer::tokenLineNumPairs> tokensToTurnInToTags);
	bool findOutIfNeedsClosingTag(std::string token);

public:
	std::vector<Tag*> getTagsAsVectorParsedFrom(IO* source);

	HTMLTagParser();
	~HTMLTagParser();
	std::vector<Tag*> parseTagsFrom(std::string fileToParseFrom); //
};


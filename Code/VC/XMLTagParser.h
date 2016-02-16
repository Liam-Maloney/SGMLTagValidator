#pragma once
#include "TagParserInterface.h"
#include "TagFactory.h"
#include "IOFactory.h"
#include <functional>
#include <queue>
#include "Lexer.h"
#include "Utility.h"
#include "SGMLParser.h"

class XMLTagParser : public TagParserInterface, public SGMLParser
{
	Tag* getTagAsObject(Lexer::tokenLineNumPairs token);
	std::vector<Tag*> formTagsAsObjects(std::vector<Lexer::tokenLineNumPairs> tokensToTurnInToTags);
	bool findOutIfNeedsClosingTag(std::string token);

public:

	XMLTagParser();
	~XMLTagParser();
	std::vector<Tag*> getTagsAsVectorParsedFrom(std::string source);
	std::vector<Tag*> parseTagsFrom(std::string fileToParseFrom);
};
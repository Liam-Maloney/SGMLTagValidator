#pragma once
#include <vector>
#include "Tag.h"
#include "Lexer.h"
#include "IO.h"

class TagParserInterface
{
protected:
	virtual Tag* getTagAsObject(Lexer::tokenLineNumPairs token) = 0;
	virtual std::vector<Tag*> formTagsAsObjects(std::vector<Lexer::tokenLineNumPairs> tokensToTurnInToTags) = 0;
	virtual bool findOutIfNeedsClosingTag(std::string token) = 0;
public:		
	virtual std::vector<Tag*> parseTagsFrom(std::string fileToParseFrom) = 0;
	virtual std::vector<Tag*> getTagsAsVectorParsedFrom(IO* source) = 0;
};
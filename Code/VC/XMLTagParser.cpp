#include "stdafx.h"
#include "XMLTagParser.h"

bool XMLTagParser::findOutIfNeedsClosingTag(std::string token)
{
	return (token[token.length() - 2] == '/') ? false : true;
}

std::vector<Tag*> XMLTagParser::parseTagsFrom(std::string fileToParseFrom)
{
	IO* source = ioHandle.getGenericIoHandle(fileToParseFrom);
	return getTagsAsVectorParsedFrom(source->getContent());
}

std::vector<Tag*> XMLTagParser::getTagsAsVectorParsedFrom(std::string source)
{
	Lexer tokenizer;
	std::vector<Lexer::tokenLineNumPairs> tokenizedTags = tokenizer.readTokenizedTagsFrom(source);
	std::vector<Tag*> parsedTags = formTagsAsObjects(tokenizedTags);
	return parsedTags;
}

Tag* XMLTagParser::getTagAsObject(Lexer::tokenLineNumPairs token)
{
	Tag* newTag = tagSupplier.getTagFor("xml");
	newTag->updateRequiresClosing(findOutIfNeedsClosingTag(token.token));
	newTag->updateIsClosing(findOutIfItIsAClosingTag(token.token));
	newTag->setTagName(findOutTagName(token.token));
	newTag->setLineNumber(token.lineNumber);
	newTag->setAttributes(findOutAttributesFrom(token.token));
	return newTag;
}

std::vector<Tag*> XMLTagParser::formTagsAsObjects(std::vector<Lexer::tokenLineNumPairs> tokensToTurnInToTags)
{
	std::vector<Tag*> tagObjects;
	for each (Lexer::tokenLineNumPairs token in tokensToTurnInToTags)
	{
		tagObjects.emplace_back(getTagAsObject(token));
	}
	return tagObjects;
}
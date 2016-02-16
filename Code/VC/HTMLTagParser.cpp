#include "stdafx.h"
#include "HTMLTagParser.h"

HTMLTagParser::HTMLTagParser()
{
	voidTags["AREA"] = true;
	voidTags["BASE"] = true;
	voidTags["BR"] = true;
	voidTags["COL"] = true;
	voidTags["COMMAND"] = true;
	voidTags["EMBED"] = true;
	voidTags["HR"] = true;
	voidTags["IMG"] = true;
	voidTags["INPUT"] = true;
	voidTags["KEYGEN"] = true;
	voidTags["LINK"] = true;
	voidTags["META"] = true;
	voidTags["PARAM"] = true;
	voidTags["SOURCE"] = true;
	voidTags["TRACK"] = true;
	voidTags["WBR"] = true;
}

HTMLTagParser::~HTMLTagParser()
{

}

std::vector<Tag*> HTMLTagParser::formTagsAsObjects(std::vector<Lexer::tokenLineNumPairs> tokensToTurnInToTags)
{
	std::vector<Tag*> tagObjects;
	for each (Lexer::tokenLineNumPairs token in tokensToTurnInToTags)
	{
		tagObjects.emplace_back(getTagAsObject(token));
	}
	return tagObjects;
}

bool HTMLTagParser::findOutIfNeedsClosingTag(std::string token)
{
	bool isClosingTag = (token[token.length() - 2] == '/') ? true : false;
	std::string tagname = findOutTagName(token);
	std::transform(tagname.begin(), tagname.end(), tagname.begin(), toupper);
	bool isVoidTag = voidTags[tagname];
	if (isClosingTag || isVoidTag)
	{
		return false;
	}
	else
	{
		return true;
	}
}

std::vector<Tag*> HTMLTagParser::parseTagsFrom(std::string fileToParseFrom)
{
	IO* source = ioHandle.getGenericIoHandle(fileToParseFrom);
	return getTagsAsVectorParsedFrom(source->getContent());
}

std::vector<Tag*> HTMLTagParser::getTagsAsVectorParsedFrom(std::string source)
{
	Lexer tokenizer;
	std::vector<Lexer::tokenLineNumPairs> tokenizedTags = tokenizer.readTokenizedTagsFrom(source);
	std::vector<Tag*> parsedTags = formTagsAsObjects(tokenizedTags);
	return parsedTags;
}

Tag* HTMLTagParser::getTagAsObject(Lexer::tokenLineNumPairs token)
{
	Tag* newTag = tagSupplier.getTagFor("html");
	newTag->updateRequiresClosing(findOutIfNeedsClosingTag(token.token));
	newTag->updateIsClosing(findOutIfItIsAClosingTag(token.token));
	newTag->setTagName(findOutTagName(token.token));
	newTag->setLineNumber(token.lineNumber);
	newTag->setAttributes(findOutAttributesFrom(token.token));
	return newTag;
}

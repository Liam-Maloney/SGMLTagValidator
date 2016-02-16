#include "stdafx.h"
#include "HTMLTagParser.h"

HTMLTagParser::HTMLTagParser()
{
	//voidTags["AREA"] = true;
	
	
	/*voidTags.emplace_back("BASE", true);
	voidTags.emplace_back("BR", true);
	voidTags.emplace_back("COL", true);
	voidTags.emplace_back("COMMAND", true);
	voidTags.emplace_back("EMBED", true);
	voidTags.emplace_back("HR", true);
	voidTags.emplace_back("IMG", true);
	voidTags.emplace_back("INPUT", true);
	voidTags.emplace_back("KEYGEN", true);
	voidTags.emplace_back("LINK", true);
	voidTags.emplace_back("META", true);
	voidTags.emplace_back("PARAM", true);
	voidTags.emplace_back("SOURCE", true);
	voidTags.emplace_back("TRACK", true);
	voidTags.emplace_back("WBR", true);*/
}

/*
bool HTMLTagParser::findOutIfNeedsClosingTag(std::string token)
{
	bool isSelfClosing = (token[token.length() - 2] == '/') ? false : true;
	//const bool is_in = voidTags[token];
	return false;
}
*/

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

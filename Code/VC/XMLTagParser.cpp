#include "stdafx.h"
#include "XMLTagParser.h"

std::queue<Tag> XMLTagParser::parseTagsFrom(std::string fileToParseFrom)
{
	std::queue<Tag> test;
	test.push(Tag("Liam"));
	return test;
}

XMLTagParser::XMLTagParser()
{

}
#include "stdafx.h"
#include "XMLTagParser.h"

std::queue<Tag> XMLTagParser::parseTagsFrom(std::string filename)
{
	std::queue<Tag> test;

	std::string tagName;
	tagName = "head";

	std::list<std::string> attributes;
	attributes.push_back("this=is");
	attributes.push_back("a=a");
	attributes.push_back("test=is");
	
	test.push(Tag(tagName, attributes));
	
	return test;
}

XMLTagParser::XMLTagParser()
{

}
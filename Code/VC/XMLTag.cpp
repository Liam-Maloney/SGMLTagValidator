#include "stdafx.h"
#include "XMLTag.h"

XMLTag::XMLTag()
{

}

XMLTag::XMLTag(std::string tagName, std::list<std::string> attributes, int lineNumber)
	: XMLTag::Tag(tagName, attributes, lineNumber)
{

}
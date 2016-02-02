#include "stdafx.h"
#include "XMLTag.h"

XMLTag::XMLTag()
{

}

XMLTag::XMLTag(std::string tagName, std::list<std::string> attributes)
	: XMLTag::Tag(tagName, attributes)
{

}
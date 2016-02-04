#include "stdafx.h"
#include "Tag.h"

Tag::Tag(std::string tagName, std::list<std::string> attributes, int lineNumber)
{
	this->tagName = tagName;
	this->attributes = attributes;
	this->lineNumber = lineNumber;
}

Tag::Tag()
	: tagName(0), attributes(0), lineNumber(0)
{}
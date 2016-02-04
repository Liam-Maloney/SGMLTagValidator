#include "stdafx.h"
#include "XMLTag.h"

void XMLTag::setAttributes(std::list<std::string> attributes)
{
	this->attributes = attributes;
}

void XMLTag::setTagName(std::string tagName)
{
	this->tagName = tagName;
}

void XMLTag::setLineNumber(int lineNumber)
{
	this->lineNumber = lineNumber;
}

std::list<std::string> XMLTag::getAttributes()
{
	return attributes;
}

std::string XMLTag::getTagName()
{
	return tagName;
}

int XMLTag::getLineNumber()
{
	return lineNumber;
}
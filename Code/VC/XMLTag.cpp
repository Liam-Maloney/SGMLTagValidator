#include "stdafx.h"
#include "XMLTag.h"


void XMLTag::updateIsClosing(bool closing)
{
	this->closing = closing;
}

void XMLTag::updateRequiresClosing(bool reqClosing)
{
	this->reqClosing = reqClosing;
}

bool XMLTag::isClosing()
{
	return closing;
}

bool XMLTag::requiresClosing()
{
	return reqClosing;
}

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
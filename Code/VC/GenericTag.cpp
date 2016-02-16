#include "stdafx.h"
#include "GenericTag.h"


void GenericTag::updateIsClosing(bool closing)
{
	this->closing = closing;
}

void GenericTag::updateRequiresClosing(bool reqClosing)
{
	this->reqClosing = reqClosing;
}

bool GenericTag::isClosing()
{
	return closing;
}

bool GenericTag::requiresClosing()
{
	return reqClosing;
}

void GenericTag::setAttributes(std::vector<Attribute> attributes)
{
	this->attributes = attributes;
}

void GenericTag::setTagName(std::string tagName)
{
	this->tagName = tagName;
}

void GenericTag::setLineNumber(int lineNumber)
{
	this->lineNumber = lineNumber;
}

std::vector<Attribute> GenericTag::getAttributes()
{
	return attributes;
}

std::string GenericTag::getTagName()
{
	return tagName;
}

int GenericTag::getLineNumber()
{
	return lineNumber;
}
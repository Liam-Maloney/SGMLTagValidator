#include "stdafx.h"
#include "Tag.h"

Tag::Tag(std::string tagName, std::list<std::string> attributes)
{
	this->tagName = tagName;
	this->attributes = attributes;
}

Tag::Tag()
	: tagName(0), attributes(0)
{}
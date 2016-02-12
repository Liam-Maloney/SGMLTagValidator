#pragma once
#include "TagValidator.h"
#include <stack>

class XMLTagValidator : public TagValidator
{
	std::list<std::string> checkTagPairs(std::list<Tag*>);
	std::list<std::string> checkAttributes(std::list<Tag*> tagsToValidate);
	std::string validateAttribute(std::list<std::string> attributes, int line, std::string tag);
	std::string valSingAttribute(std::string currentAttr, int line, std::string tag);
public:
	std::list<std::string> validateTags(std::list<Tag*>);
	XMLTagValidator();
	~XMLTagValidator();
};
#pragma once
#include "TagValidator.h"
#include <stack>

class XMLTagValidator : public TagValidator
{
	std::list<std::string> checkTagPairs(std::list<Tag*>);
	std::list<std::string> checkAttributes(std::list<Tag*> tagsToValidate);
	std::string validateAttribute(std::list<std::string> attributes);
	std::string valSingAttribute(std::string currentAttr);
public:
	std::list<std::string> validateTags(std::list<Tag*>);
	XMLTagValidator();
	~XMLTagValidator();
};
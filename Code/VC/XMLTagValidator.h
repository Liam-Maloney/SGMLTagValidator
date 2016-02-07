#pragma once
#include "TagValidator.h"
#include <stack>

class XMLTagValidator : public TagValidator
{
	std::list<std::string> checkTagPairs(std::list<Tag*>);
public:
	std::list<std::string> validateTags(std::list<Tag*>);
	XMLTagValidator();
	~XMLTagValidator();
};
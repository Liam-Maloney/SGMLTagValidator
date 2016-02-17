#pragma once
#include "TagValidator.h"
#include <stack>
#include <vector>

class SGMLTagValidator : public TagValidator
{
	std::vector<std::string> checkTagPairs(std::vector<Tag*>);
	std::vector<std::string> checkAttributes(std::vector<Tag*> tagsToValidate);
	std::string validateAttribute(std::vector<Attribute> attributes, int line, std::string tag);
	std::string valSingAttribute(Attribute currentAttr, int line, std::string tag);
public:
	std::vector<std::string> validateTags(std::vector<Tag*>);
};
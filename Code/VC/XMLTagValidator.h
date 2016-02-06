#pragma once
#include "TagValidator.h"

class XMLTagValidator : public TagValidator
{
public:
	std::list<std::string> validateTags(std::list<Tag*>);
	XMLTagValidator();
	~XMLTagValidator();
};


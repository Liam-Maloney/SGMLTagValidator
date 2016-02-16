#pragma once
#include <string>
#include "TagValidator.h"
#include "XMLTagValidator.h"

class ValidatorFactory
{
public:
	TagValidator* createValidatorFor(std::string tagType);
};
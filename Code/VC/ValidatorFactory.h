#pragma once
#include <string>
#include "TagValidator.h"
#include "SGMLTagValidator.h"

class ValidatorFactory
{
public:
	TagValidator* createValidatorFor(std::string tagType);
};


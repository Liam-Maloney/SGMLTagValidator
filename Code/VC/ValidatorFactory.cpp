#include "stdafx.h"
#include "ValidatorFactory.h"

TagValidator* ValidatorFactory::createValidatorFor(std::string tagType)
{
	return new XMLTagValidator();
}
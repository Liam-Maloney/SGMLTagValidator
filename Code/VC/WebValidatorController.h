#pragma once
#include "ParserFactory.h"

class WebValidator
{
	ParserFactory parserSupplier;
	std::string fileExtensionTypeOf(std::string fileToValidate);
public:
	std::string getValidationReportFor(std::string fileToValidate);
};
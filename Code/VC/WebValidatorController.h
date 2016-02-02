#pragma once
#include "ParserFactory.h"
#include "gtest\gtest.h"
#include <fstream>
#include <iostream>

class WebValidator
{
	ParserFactory parserSupplier;
	std::string fileExtensionTypeOf(std::string fileToValidate);
public:
	std::string getValidationReportFor(std::string fileToValidate);
};
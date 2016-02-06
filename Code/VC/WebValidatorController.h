#pragma once
#include "stdafx.h"
#include "ParserFactory.h"
#include "gtest\gtest.h"
#include <iostream>
#include <fstream>

class WebValidator
{
	ParserFactory parserSupplier;
	std::string fileExtensionTypeOf(std::string fileToValidate);
public:
	std::string getValidationReportFor(std::string fileToValidate);
};
#pragma once
#include "stdafx.h"
#include "ParserFactory.h"
#include "gtest\gtest.h"
#include <iostream>
#include <fstream>
#include "ValidatorFactory.h"
#include "TagValidator.h"

class WebValidator
{
	ParserFactory parserSupplier;
	ValidatorFactory validator;

	std::string fileExtensionTypeOf(std::string fileToValidate);
public:
	std::vector<std::string> getValidationReportFor(std::string fileToValidate);
};
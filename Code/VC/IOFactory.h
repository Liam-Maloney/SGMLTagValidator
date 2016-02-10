#pragma once
#include "stdafx.h"
#include "FileParser.h"
#include <string>
#include "IO.h"

class IOFactory
{
	IO* ioHandle;
public:
	~IOFactory();

	std::string fileExtensionTypeOf(std::string fileToValidate);
	IO* getGenericIoHandle(std::string forThisTypeOfInput);
};
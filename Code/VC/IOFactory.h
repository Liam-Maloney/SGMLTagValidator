#pragma once
#include "stdafx.h"
#include "FileParser.h"
#include <string>
#include "IO.h"

class IOFactory
{
	IO* ioHandle;
	std::string fileExtensionTypeOf(std::string fileToValidate);
public:
	~IOFactory();
	IO* getGenericIoHandle(std::string forThisTypeOfInput);
};
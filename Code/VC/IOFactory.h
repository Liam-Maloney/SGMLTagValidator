#pragma once
#include "stdafx.h"
#include "FileParser.h"
#include <string>
#include "IO.h"

class IOFactory
{
	IO* ioHandle;
public:
	IO* getGenericIoHandle(std::string forThisTypeOfInput);
};
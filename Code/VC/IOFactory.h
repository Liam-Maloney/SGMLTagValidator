#pragma once
#include "stdafx.h"
#include "FileParser.h"
#include <string>
#include "IO.h"

class IOFactory
{
public:
	IO* getGenericIoHandle(std::string forThisTypeOfInput);
};
#pragma once
#include "stdafx.h"
#include <string>
#include "XMLTag.h"

class Tag;

class TagFactory
{
public:
	Tag* getTagFor(std::string syntaxType);
};


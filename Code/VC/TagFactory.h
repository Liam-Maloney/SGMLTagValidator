#pragma once
#include "stdafx.h"
#include "Tag.h"

class TagFactory
{
public:
	Tag* getTagFor(std::string syntaxType);
};


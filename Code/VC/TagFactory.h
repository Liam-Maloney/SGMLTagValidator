#pragma once
#include "stdafx.h"
#include <string>
#include "XMLTag.h"
#include "HTMLTag.h"

class Tag;

class TagFactory
{
	Tag* tagHandle;
public:
	~TagFactory();
	Tag* getTagFor(std::string syntaxType);
};


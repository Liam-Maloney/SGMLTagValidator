#include "stdafx.h"
#include "TagFactory.h"

Tag* TagFactory::getTagFor(std::string syntaxType)
{
	if (syntaxType == "xml")
	{
		return new XMLTag();
	}
	else if(syntaxType == "html")
	{
		return new HTMLTag();
	}
	else
	{
		return NULL;
	}
}
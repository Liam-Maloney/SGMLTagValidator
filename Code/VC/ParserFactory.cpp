#include "stdafx.h"
#include "ParserFactory.h"

TagParser* ParserFactory::createParserFor(std::string fileType)
{
	if (fileType == "xml")
	{
		return new XMLTagParser();
	}
	else
	{
		return NULL;
	}
}

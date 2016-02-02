#include "stdafx.h"
#include "ParserFactory.h"
#include "XMLTagParser.h"

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

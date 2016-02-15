#include "stdafx.h"
#include "ParserFactory.h"

TagParser* ParserFactory::createParserFor(std::string fileType)
{
	std::transform(fileType.begin(), fileType.end(), fileType.begin(), toupper);
	if (fileType == "XML")
	{
		return new XMLTagParser();
	}
	else if (fileType == "HTML")
	{
		return NULL;
	}
	else
	{
		throw std::runtime_error("The file type " +
			fileType + " is not supported.  Please enter html or xml files.\n\n");
	}
}

ParserFactory::~ParserFactory()
{
	delete parserHandle;
}
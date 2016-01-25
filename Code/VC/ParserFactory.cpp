#include "stdafx.h"
#include "ParserFactory.h"
#include "XMLTagParser.h"

TagParser* ParserFactory::createParserFor(std::string typeOfParserToCreate)
{
	if (typeOfParserToCreate == "xml")
	{
		return new XMLTagParser();
	}
	else {
		return NULL;
	}
}

ParserFactory::ParserFactory()
{
}


ParserFactory::~ParserFactory()
{
}

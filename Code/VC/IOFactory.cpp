#include "stdafx.h"
#include "IOFactory.h"

IO* IOFactory::getGenericIoHandle(std::string inputSource)
{
	if (inputSource == "test.xml")
	{
		return new FileParser(inputSource);
	}
	else
	{
		return NULL;
	}
}
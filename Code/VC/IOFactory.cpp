#include "stdafx.h"
#include "IOFactory.h"

IO* IOFactory::getGenericIoHandle(std::string inputSource)
{
	return new FileParser(inputSource);
}



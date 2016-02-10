#include "stdafx.h"
#include "IOFactory.h"

//TODO: REMOVE THIS AND HAVE IT GO ELSEWHERE
std::string IOFactory::fileExtensionTypeOf(std::string fileToValidate)
{
	size_t indexOfBeginningOfFileExtention = fileToValidate.find_last_of(".");
	return fileToValidate.substr(++indexOfBeginningOfFileExtention, fileToValidate.length() - indexOfBeginningOfFileExtention);
}

IO* IOFactory::getGenericIoHandle(std::string inputSource)
{

	if (fileExtensionTypeOf(inputSource) == "xml")
	{
		return new FileParser(inputSource);
	}
	else
	{
		return NULL;
	}
}

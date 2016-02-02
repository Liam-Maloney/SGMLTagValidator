#include "stdafx.h"
#include "WebValidatorController.h"

std::string WebValidator::fileExtensionTypeOf(std::string fileToValidate)
{
	size_t indexOfBeginningOfFileExtention = fileToValidate.find_last_of(".");
	return fileToValidate.substr(++indexOfBeginningOfFileExtention);
}

std::string WebValidator::getValidationReportFor(std::string fileToValidate) 
{
	TagParser* polymorphicParser = parserSupplier.createParserFor(fileExtensionTypeOf(fileToValidate));
	std::queue<Tag> parsedTags = polymorphicParser->parseTagsFrom(fileToValidate);
	//TODO: Pass off the returned queue of items to be validated.
	return "Nothing For Now";
}

void main()
{
	WebValidator webDocValidator;
	std::string validationReport = webDocValidator.getValidationReportFor("test.xml");
}
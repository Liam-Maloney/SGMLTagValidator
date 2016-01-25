#include "stdafx.h"
#include "WebValidatorController.h"

std::string WebValidator::fileExtensionTypeOf(std::string fileToValidate)
{
	size_t indexOfBeginningOfFileExtention = fileToValidate.find_last_of(".");
	return fileToValidate.substr(++indexOfBeginningOfFileExtention);

}

std::string WebValidator::getValidationReportFor(std::string fileToValidate) {

	TagParser* polyMorphicParser = parserSupplier.createParserFor(fileExtensionTypeOf(fileToValidate));
	std::queue<Tag> parsedTags = polyMorphicParser->parseTagsFrom(fileToValidate);
	return "Nothing For Now";
}

int main()
{
	WebValidator webDocValidator;
	std::string validationReport = webDocValidator.getValidationReportFor("test.xml");
	return 0;
}
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
	std::list<Tag*> parsedTags;
	try
	{
		std::list<Tag*> parsedTags = polymorphicParser->parseTagsFrom(fileToValidate);
	}
	catch (std::exception &cause)
	{
		std::cout << "Error: " << cause.what();
	}
	//TODO: Pass off the returned queue of items to be validated.
	TagValidator* polymorphicValidator = validator.createValidatorFor(fileExtensionTypeOf(fileToValidate));
	std::list<std::string> errors = polymorphicValidator->validateTags(parsedTags);
	return "Nothing For Now";
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	WebValidator validator;
	validator.getValidationReportFor("test.xml");

	return RUN_ALL_TESTS();
}
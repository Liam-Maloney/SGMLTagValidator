#include "stdafx.h"
#include "WebValidatorController.h"

std::string WebValidator::fileExtensionTypeOf(std::string fileToValidate)
{
	size_t indexOfBeginningOfFileExtention = fileToValidate.find_last_of(".");
	return fileToValidate.substr(++indexOfBeginningOfFileExtention, fileToValidate.length() - indexOfBeginningOfFileExtention);
}

std::vector<std::string> WebValidator::getValidationReportFor(std::string fileToValidate) 
{
	TagParser* polymorphicParser = parserSupplier.createParserFor(fileExtensionTypeOf(fileToValidate));
	std::vector<Tag*> parsedTags;
	try
	{
		parsedTags = polymorphicParser->parseTagsFrom(fileToValidate);
	}
	catch (std::exception &cause)
	{
		std::cout << "Error: " << cause.what();
	}
	//TODO: Pass off the returned queue of items to be validated.
	TagValidator* polymorphicValidator = validator.createValidatorFor(fileExtensionTypeOf(fileToValidate));
	std::vector<std::string> errors = polymorphicValidator->validateTags(parsedTags);
	return errors;
}

int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);

	WebValidator validator;
	std::vector<std::string> validationReport = validator.getValidationReportFor("test.xml");

	for each (std::string reportItem in validationReport)
	{
		std::cout << reportItem << std::endl;
	}

	std::cout << "\n\n\n\n\n" << std::endl;
	system("pause");
	return RUN_ALL_TESTS();
}
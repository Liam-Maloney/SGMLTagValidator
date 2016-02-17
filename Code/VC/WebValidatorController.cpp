#include "stdafx.h"
#include "WebValidatorController.h"

std::string WebValidator::fileExtensionTypeOf(std::string fileToValidate)
{
	size_t indexOfBeginningOfFileExtention = fileToValidate.find_last_of(".");
	if (indexOfBeginningOfFileExtention == -1)
	{
		throw std::runtime_error("File name you entered (" + fileToValidate + 
			") has no extension.\nThe system currently supports both xml and html files.\n\n\n");
	}

	std::string extension = fileToValidate.substr(++indexOfBeginningOfFileExtention,
		fileToValidate.length() - indexOfBeginningOfFileExtention);

	std::transform(extension.begin(), extension.end(), extension.begin(), toupper);

	if (extension == "XML" || extension == "HTML")
	{
		return extension;
	}
	else
	{
		throw std::runtime_error("File name you entered (" + fileToValidate +
			") is not a valid file type.\nThe system currently supports only xml and html files.\n\n\n");
	}
}

std::vector<std::string> WebValidator::getValidationReportFor(std::string fileToValidate) 
{
	TagParserInterface* polymorphicParser = parserSupplier.createParserFor(
		fileExtensionTypeOf(fileToValidate));
	std::vector<Tag*> parsedTags;
	parsedTags = polymorphicParser->parseTagsFrom(fileToValidate);
	TagValidator* polymorphicValidator = validator.createValidatorFor(
		fileExtensionTypeOf(fileToValidate));
	std::vector<std::string> errors = 
		polymorphicValidator->validateTags(parsedTags);
	delete polymorphicParser;
	delete polymorphicValidator;

	for each (Tag* current in parsedTags)
	{
		delete current;
	}

	return errors;
}

void clear()
{
	for (int i = 0; i < 200; i++)
		std::cout << std::endl;
}

void executeProgram()
{
	clear();
	int option = 0;
	while (true)
	{
		std::cout << "Welcome to the XML/HTML Validator! \n" << std::endl;
		std::cout << std::endl;
		std::cout << "\tPlease Select from one of the following options:\n  " << std::endl;
		std::cout << "\t\t1. Validate a xml/html file." << std::endl;
		std::cout << "\t\t2. Exit" << std::endl;
		std::cout << std::endl;
		std::cout << std::endl;
		std::cout << "Your Choice: ";
		std::cin >> option;

		switch (option)
		{
		case 1:
		{
			try
			{
				clear();
				WebValidator validator;
				std::string fileToValidate;
				std::cout << "Please enter the name of the file to validate (eg: test.html, test.xml)\n" << std::endl;
				std::cin >> fileToValidate;
				clear();
				std::vector<std::string> validationReport = validator.getValidationReportFor(fileToValidate);

				for each (std::string reportItem in validationReport)
				{
					std::cout << reportItem << std::endl;
				}
				std::cout << "\n\n\n";
				system("pause");
				clear();
			}
			catch (std::exception &cause)
			{
				std::cout << "Error: " << cause.what();
			}
			break;
		}

		case 2:
			exit(0);
			break;
		default:
		{
			clear();
			std::cout << "ERROR:  Please enter a valid option (1 for validator, 2 for Exit.)" << std::endl;
			system("pause");
			break;
		}
		}
	}
}
int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	executeProgram();
	return RUN_ALL_TESTS();
}
#include "stdafx.h"
#include "XMLTagValidator.h"

std::string XMLTagValidator::valSingAttribute(std::string currentAttr)
{
	std::string result = "";

	//finished here, need to add in more conditionals for all the possible errors in the source
	if (currentAttr[0] == '=')
	{
		result += "No name for tag: " + currentAttr + "\n";
	}

	

	return result;
}

std::string XMLTagValidator::validateAttribute(std::list<std::string> attributes)
{
	std::string results = "";

	for each (std::string currentAttr in attributes)
	{
		results += valSingAttribute(currentAttr);
	}

	return results;
}

std::list<std::string> XMLTagValidator::checkAttributes(std::list<Tag*> tagsToValidate)
{
	std::list<std::string> results;

	for each(Tag* current in tagsToValidate)
	{
		results.emplace_back(validateAttribute(current->getAttributes()));
	}

	return results;
}

std::list<std::string> XMLTagValidator::validateTags(std::list<Tag*> tagsToValidate)
{
	//TODO: Element names cannot start with the letters xml (or XML, or Xml, etc)
	//TODO: XML TAGS MUST START WITH AN _ OR A LETTER
	//TODO: POSSIBLY RETURN THIS TO A VIEW LAYER FROM THE CONTROLLER IN LIST FORMAT
	std::list<std::string> results;
	results.emplace_back("Tag open/close Validation Results: \n\n");
	std::list<std::string> tagResults = checkTagPairs(tagsToValidate);
	results.splice(results.end(), tagResults);
	results.emplace_back("\n\nValidation of tag attributes: \n\n");
	std::list<std::string> attributeResults = checkAttributes(tagsToValidate);
	results.splice(results.end(), attributeResults);
	return results;
}

std::list<std::string> XMLTagValidator::checkTagPairs(std::list<Tag*> tagsToCheck)
{
	std::stack<Tag*> bracketPairs;
	std::list<std::string> results;
	for each (Tag* current in tagsToCheck)
	{
		if (current->isClosing())
		{
			
			if (bracketPairs.top()->getTagName() != current->getTagName())
			{
				//TODO: ADD IN A METHOD TO HANDLE THE ERROR REPORTING
				results.emplace_back("Error line " + std::to_string(bracketPairs.top()->getLineNumber()) + ", no closing tag for <" + bracketPairs.top()->getTagName() + ">\n");
				results.emplace_back("Error line " + std::to_string(current->getLineNumber()) + ", no opening tag for <" + current->getTagName() + ">, expected closing tag for <" + bracketPairs.top()->getTagName() + ">\n");
			}
			bracketPairs.pop();

		}
		else
		{
			if (current->requiresClosing() == false)
			{
				//do nothing because it does not need to be validated as it does not require a closing
			}
			else
			{
				bracketPairs.push(current);
			}
		}
	}
	return results;
}

XMLTagValidator::XMLTagValidator()
{
}


XMLTagValidator::~XMLTagValidator()
{
}

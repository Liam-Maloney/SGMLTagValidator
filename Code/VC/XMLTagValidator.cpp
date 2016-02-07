#include "stdafx.h"
#include "XMLTagValidator.h"

std::list<std::string> XMLTagValidator::validateTags(std::list<Tag*> tagsToValidate)
{
	//TODO: POSSIBLY RETURN THIS TO A VIEW LAYER FROM THE CONTROLLER IN LIST FORMAT
	std::list<std::string> results;
	results.emplace_back("Tag open/close Validation Results: \n\n");
	std::list<std::string> tagResults = checkTagPairs(tagsToValidate);
	results.splice(results.end(), tagResults);
	results.emplace_back("\n\nValidation of tag attributes: \n\n");
	std::list<std::string> attributeResults = checkAttributes(tagsToValidate);
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

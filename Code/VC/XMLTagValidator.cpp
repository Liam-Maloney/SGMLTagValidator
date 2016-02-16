#include "stdafx.h"
#include "XMLTagValidator.h"

std::string XMLTagValidator::valSingAttribute(Attribute currentAttr, int line, std::string tag)
{
	std::string result = "";

	if (currentAttr.hasEquals)
	{
		if (!currentAttr.hasName)
		{
			if (currentAttr.hasValue)
			{
				result += "Tag <" + tag + ">, line " + std::to_string(line) + ", no Name for =" + currentAttr.value + "\n";
			}
			else
			{
				result += "Tag <" + tag + ">, line " + std::to_string(line) + ", has a stray ='s\n";
			}
		}
		else if (!currentAttr.hasValue)
		{
			result += "Tag <" + tag + ">, line " + std::to_string(line) + " has no value for " + currentAttr.name + "\n";
		}
	}
	else
	{
		if (currentAttr.hasName)
		{
			result += "Attribute in tag <" + tag + "> on line " + std::to_string(line) + " has no value for " + currentAttr.name + "\n";
		}
		else if (currentAttr.hasValue)
		{
			result += "Attribute in tag <" + tag + "> on line " + std::to_string(line) + " has no name or equals for " + currentAttr.value + "\n";
		}
	}
	
	return result;
}

std::string XMLTagValidator::validateAttribute(std::vector<Attribute> attributes, int line, std::string tag)
{
	std::string results = "";

	for each (Attribute currentAttr in attributes)
	{
		results += valSingAttribute(currentAttr, line, tag);
	}

	return results;
}

std::vector<std::string> XMLTagValidator::checkAttributes(std::vector<Tag*> tagsToValidate)
{
	std::vector<std::string> results;
	for each(Tag* current in tagsToValidate)
	{
		if (current->isClosing() && !current->getAttributes().empty())
		{
			results.emplace_back("Closing attribute " + current->getTagName() + 
				" on line " + std::to_string(current->getLineNumber()) + 
				" should not have attributes (Attributes not allowed in XML closing tags)");
		}
		else
		{
			std::string tag = current->getTagName();
			int line = current->getLineNumber();
			std::string result = validateAttribute(current->getAttributes(), line, tag);
			if (result.size() != 0)
			{
				results.emplace_back(result);
			}
		}
	}

	return results;
}

std::vector<std::string> XMLTagValidator::validateTags(std::vector<Tag*> tagsToValidate)
{
	std::vector<std::string> results;
	results.emplace_back("Tag open/close Validation Results: \n\n");
	std::vector<std::string> tagResults = checkTagPairs(tagsToValidate);
	results.insert(results.end(), tagResults.begin(), tagResults.end());
	results.emplace_back("\n\nValidation of tag attributes: \n\n");
	std::vector<std::string> attributeResults = checkAttributes(tagsToValidate);
	results.insert(results.end(), attributeResults.begin(), attributeResults.end());
	return results;
}

std::vector<std::string> XMLTagValidator::checkTagPairs(std::vector<Tag*> tagsToCheck)
{
	std::stack<Tag*> bracketPairs;
	std::vector<std::string> results;
	for each (Tag* current in tagsToCheck)
	{
		if (current->isClosing())
		{
			if (bracketPairs.top()->getTagName() != current->getTagName())
			{
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
#include "stdafx.h"
#include "SGMLTagValidator.h"

std::string SGMLTagValidator::valSingAttribute(Attribute currentAttr, int line, std::string tag)
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

std::string SGMLTagValidator::validateAttribute(std::vector<Attribute> attributes, int line, std::string tag)
{
	std::string results = "";

	for each (Attribute currentAttr in attributes)
	{
		results += valSingAttribute(currentAttr, line, tag);
	}

	return results;
}

std::vector<std::string> SGMLTagValidator::checkAttributes(std::vector<Tag*> tagsToValidate)
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

std::vector<std::string> SGMLTagValidator::validateTags(std::vector<Tag*> tagsToValidate)
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

std::vector<std::string> SGMLTagValidator::checkTagPairs(std::vector<Tag*> tagsToCheck)
{
	std::stack<std::string> bracketPairs;
	std::string tagName;
	std::string openTagOnStack;
	bool isClosingTag;

	std::vector<std::string> results;
	for each (Tag* current in tagsToCheck)
	{
		tagName = current->getTagName();
		isClosingTag = current->isClosing();

		if (isClosingTag && !bracketPairs.empty())
		{
			openTagOnStack = bracketPairs.top();
			if (openTagOnStack == tagName)
			{
				bracketPairs.pop();
			}
			else
			{
				results.emplace_back("No closing tag for " + openTagOnStack + ", encountered closing tag for "
					+ tagName + " on line " + std::to_string(current->getLineNumber()));
				bracketPairs.pop();
				if (!bracketPairs.empty())
				{
					bracketPairs.pop();
				}
			}
		}
		else
		{
			if (current->requiresClosing())
			{
				bracketPairs.push(tagName);
			}
		}
	}

	while(!bracketPairs.empty())
	{
		tagName = bracketPairs.top();
		results.emplace_back("No closing tag for " + tagName + ", encountered closing tag for "
			+ tagName + " at EOF");
		bracketPairs.pop();
	}

	return results;
}
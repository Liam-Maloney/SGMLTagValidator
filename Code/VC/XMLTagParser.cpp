#include "stdafx.h"
#include "XMLTagParser.h"

std::vector<Tag*> XMLTagParser::parseTagsFrom(std::string fileToParseFrom)
{
	IO* source = ioHandle.getGenericIoHandle(fileToParseFrom);
	return getTagsAsVectorParsedFrom(source->getContent());
}

std::vector<Tag*> XMLTagParser::getTagsAsVectorParsedFrom(std::string source)
{
	Lexer tokenizer;
	std::vector<Lexer::tokenLineNumPairs> tokenizedTags = tokenizer.readTokenizedTagsFrom(source);
	std::vector<Tag*> parsedTags = formTagsAsObjects(tokenizedTags);
	return parsedTags;
}

std::vector<Tag*> XMLTagParser::formTagsAsObjects(std::vector<Lexer::tokenLineNumPairs> tokensToTurnInToTags)
{
	std::vector<Tag*> tagObjects;
	for each (Lexer::tokenLineNumPairs token in tokensToTurnInToTags)
	{
		tagObjects.emplace_back(getTagAsObject(token));
	}
	return tagObjects;
}

bool XMLTagParser::findOutIfNeedsClosingTag(std::string token)
{
	return (token[token.length() - 2] == '/') ? false : true;
}

bool XMLTagParser::findOutIfItIsAClosingTag(std::string token)
{
	return (token[1] == '/') ? true : false;
}

std::string XMLTagParser::findOutTagName(std::string token)
{
	int firstSpaceIndex = token.find_first_of(' ');
	if (findOutIfItIsAClosingTag(token))
	{
		if (firstSpaceIndex == -1)
		{
			return token.substr(2, token.length() - 3);
		}
		else
		{
			return token.substr(2, firstSpaceIndex - 3);
		}
	}
	else
	{
		if (firstSpaceIndex != -1)
		{
			return token.substr(1, firstSpaceIndex - 1);
		}
		else
		{
			return token.substr(1, token.length() - 2);
		}
	}
}

std::string XMLTagParser::getNextAttributeString(std::string isolatedAttributes)
{
	int endOfNextAttribute = getEndIndexOfAttribute(isolatedAttributes);
	return isolatedAttributes.substr(0, endOfNextAttribute + 1);
}

std::string XMLTagParser::parseAttributeName(std::string fullAttribute)
{
	int indexOfEquals = fullAttribute.find_first_of('=');
	if (indexOfEquals == -1)
	{
		if (fullAttribute[0] != '"')
		{
			return fullAttribute.substr(0, fullAttribute.length());
		}
		else
		{
			return "";
		}
	}
	else
	{
		return fullAttribute.substr(0, indexOfEquals);
	}
}

bool XMLTagParser::hasValue(std::string fullAttribute)
{
	if (hasEquals(fullAttribute))
	{
		int equalsIndex = fullAttribute.find_first_of('=');
		if (equalsIndex == fullAttribute.length() - 1)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		if (fullAttribute[0] == '"')
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool XMLTagParser::hasEquals(std::string fullAttribute)
{
	int indexOfQuote = fullAttribute.find_first_of('"');
	indexOfQuote = (indexOfQuote == -1) ? fullAttribute.length() : indexOfQuote;
	int indexOfEquals = fullAttribute.find_first_of('=');
	indexOfEquals = (indexOfEquals == -1) ? fullAttribute.length() : indexOfEquals;
	return (indexOfEquals < indexOfQuote) ? true : false;
}

std::string XMLTagParser::parseAttributeValue(std::string fullAttribute)
{
	if (hasValue(fullAttribute))
	{
		int equalsIndex = fullAttribute.find_first_of('=');
		return fullAttribute.substr(equalsIndex + 1, fullAttribute.length());
	}
	else
	{
		return "";
	}
}

Attribute XMLTagParser::constructAttribute(std::string singleAattributeAsString)
{
	Attribute attributeAsObject;
	attributeAsObject.name = parseAttributeName(singleAattributeAsString);
	attributeAsObject.hasEquals = hasEquals(singleAattributeAsString);
	attributeAsObject.hasValue = hasValue(singleAattributeAsString);
	attributeAsObject.hasName = (attributeAsObject.name == "") ? false : true;
	attributeAsObject.value = parseAttributeValue(singleAattributeAsString);
	return attributeAsObject;
}

int XMLTagParser::getEndIndexOfAttribute(std::string isolatedAttributes)
{
	std::vector<int> ignoreSpacesBetween = Utility::findLocationsOf('"', isolatedAttributes);
	int endOfNextAttribute;
	if (ignoreSpacesBetween.size() <= 1)
	{
		endOfNextAttribute = isolatedAttributes.length();
	}
	else
	{
		ignoreSpacesBetween.erase(ignoreSpacesBetween.begin());
		endOfNextAttribute = ignoreSpacesBetween.front();
	}
	return endOfNextAttribute;
}

std::string XMLTagParser::popFirstAttribute(std::string isolatedAttributes)
{
	int removeAllBefore = getEndIndexOfAttribute(isolatedAttributes) + 1;
	if (removeAllBefore == isolatedAttributes.length())
	{
		return "";
	}
	else
	{
		return isolatedAttributes.substr(++removeAllBefore);
	}
}

std::vector<std::string> XMLTagParser::getAllSubstringsBetweenSpaces(
	std::vector<int> *indexOfSpaces,
	std::string* isolatedAttributes)
{
	std::vector<std::string> substrings;
	if (indexOfSpaces->empty())
	{
		substrings.emplace_back(*isolatedAttributes);
		return substrings;
	}

	int startIndex = indexOfSpaces->front();

	int indexForFirstSubstringToTakeAtEnd = startIndex;

	indexOfSpaces->erase(indexOfSpaces->begin());
	int endIndex;

	while (!indexOfSpaces->empty())
	{
		if (!indexOfSpaces->empty())
		{
			endIndex = indexOfSpaces->front();
		}
		else
		{
			endIndex = isolatedAttributes->length() - 1;
		}
		substrings.emplace_back(isolatedAttributes->substr(
			startIndex + 1, endIndex - (startIndex + 1)));

		startIndex = indexOfSpaces->front();
		indexOfSpaces->erase(indexOfSpaces->begin());
	}

	substrings.emplace(substrings.begin(), isolatedAttributes->substr(0, indexForFirstSubstringToTakeAtEnd));
	substrings.emplace_back(isolatedAttributes->substr(startIndex + 1, isolatedAttributes->length() - startIndex));

	return substrings;
}

std::vector<std::string> XMLTagParser::removeRedundantEntries(std::vector<std::string> substrings)
{
	std::vector<std::string> isolatedAttributesAsVector;
	for each (std::string current in substrings)
	{
		if (current != "")
		{
			isolatedAttributesAsVector.emplace_back(current);
		}
	}
	return isolatedAttributesAsVector;
}

std::vector<int> XMLTagParser::getIndexOfEqualsInVector(std::vector<std::string>* substrings)
{
	std::vector<int> locationsOfEquals;
	int i = 0;
	
	for (int i = 0; i < substrings->size(); i++)
	{
		// finds "=", "xxxxx=", and '="xxxxx"'
		if ((*substrings)[i] == "=" || (*substrings)[i][0] == '=' || (*substrings)[i][(*substrings)[i].length()-1] == '=')
		{
			locationsOfEquals.emplace_back(i);
		}
	}
	return locationsOfEquals;
}

bool XMLTagParser::notAlreadyPartOfAttribute(int index, std::vector<std::string> substrings)
{
	if (substrings[index].find_first_of('=') == -1)
	{
		if (substrings[index].find_first_of('"') == -1)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool XMLTagParser::rightOfThisIsValue(int index, std::vector<std::string> substrings)
{
	if (substrings.size() == 1 || 0)
	{
		return false;
	}

	if (substrings[index + 1][0] == '"')
	{
		if (notAlreadyPartOfAttribute(index, substrings))
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

bool XMLTagParser::leftOfThisIsName(int index, std::vector<std::string> substrings)
{
	if (substrings.size() == 0)
	{
		return false;
	}

	if (isalpha(substrings[index - 1][0]))
	{
		if (notAlreadyPartOfAttribute(index, substrings))
		{
			return true;
		}
	}
	else
	{
		return false;
	}
}

void XMLTagParser::concatRightValueToThisEquals(int indexOfEqualsToConcatTo, std::vector<std::string>* substrings)
{
	std::vector<std::string>::iterator concatAndEraseThis = substrings->begin();
	for (int i = 0; i < indexOfEqualsToConcatTo; i++)
	{
		concatAndEraseThis++;
	}
	concatAndEraseThis++;
	std::string valueToConcat = *concatAndEraseThis;
	concatAndEraseThis--;
	*concatAndEraseThis += valueToConcat;
	concatAndEraseThis++;
	substrings->erase(concatAndEraseThis);
}

void XMLTagParser::concatLeftNameToThisEquals(int indexOfEqualsToConcatTo, std::vector<std::string>* substrings)
{
	std::vector<std::string>::iterator concatAndEraseThis = substrings->begin();
	for (int i = 0; i < indexOfEqualsToConcatTo; i++)
	{
		concatAndEraseThis++;
	}
	concatAndEraseThis--;
	std::string nameToConcat = *concatAndEraseThis;
	concatAndEraseThis++;
	*concatAndEraseThis = nameToConcat + *concatAndEraseThis;
	concatAndEraseThis--;
	substrings->erase(concatAndEraseThis);
}

std::vector<std::string> XMLTagParser::removeSpaceAroundNameValuePairs(
	std::vector<std::string> substrings)
{
	std::vector<int> allEqualsInTag = getIndexOfEqualsInVector(&substrings);
	for (int i = allEqualsInTag.size() - 1; i >= 0; i--)
	{
		if (allEqualsInTag[i] == 0)
		{
			if (rightOfThisIsValue(allEqualsInTag[i], substrings) && substrings[allEqualsInTag[i] + 1] != "=")
			{
				concatRightValueToThisEquals(allEqualsInTag[i], &substrings);
			}
		}
		else if (allEqualsInTag[i] == substrings.size() - 1 && substrings[allEqualsInTag[i] -1] != "=")
		{
			if (leftOfThisIsName(allEqualsInTag[i], substrings))
			{
				concatLeftNameToThisEquals(allEqualsInTag[i], &substrings);
			}
		}
		else
		{
			if (rightOfThisIsValue(allEqualsInTag[i], substrings)) //&& substrings[allEqualsInTag[i] + 1] != "=")
			{
				if (substrings[allEqualsInTag[i]][substrings[allEqualsInTag[i]].length() - 1] != '"')
				{
					concatRightValueToThisEquals(allEqualsInTag[i], &substrings);
				}
			}
			if (leftOfThisIsName(allEqualsInTag[i], substrings) && substrings[allEqualsInTag[i] - 1] != "=" && 
				substrings[allEqualsInTag[i]][substrings[allEqualsInTag[i]].length() - 1] != '=')
			{
				if (substrings[allEqualsInTag[i] - 1][substrings[allEqualsInTag[i] - 1].length() - 1] != '=' &&
					substrings[allEqualsInTag[i] - 1][substrings[allEqualsInTag[i] - 1].length() - 1] != '"')
				concatLeftNameToThisEquals(allEqualsInTag[i], &substrings);
			}
		}
	}
	return substrings;
}

std::vector<std::string> XMLTagParser::removeRepeatedSpacesIn(std::string* isolatedAttributes)
{
	std::vector<int> indexOfSpaces = Utility::findLocationsOf(' ', *isolatedAttributes);
	std::vector<std::string> substrings;
	if (indexOfSpaces.empty())
	{
		substrings.emplace_back(*isolatedAttributes);
	}
	else
	{
		Utility::removeDelimitersBetweenQuotesIn(&indexOfSpaces, *isolatedAttributes);
		substrings = getAllSubstringsBetweenSpaces(
			&indexOfSpaces, isolatedAttributes);
		substrings = removeRedundantEntries(substrings);
		substrings = removeSpaceAroundNameValuePairs(substrings);
	}
	return substrings;
};

std::vector<Attribute> XMLTagParser::parseAttributesAsObjectsFrom(std::string isolatedAttributes)
{
	std::vector<Attribute> tagAttributes;
	std::vector<std::string> nameValuePairs = removeRepeatedSpacesIn(&isolatedAttributes);

	while (!nameValuePairs.empty())
	{
		std::string makeAttributeObjectFromThis = nameValuePairs.front();
		nameValuePairs.erase(nameValuePairs.begin());
		Attribute nextAttribute = constructAttribute(makeAttributeObjectFromThis);
		tagAttributes.emplace_back(nextAttribute);
	}

	if (tagAttributes.size() == 1)
	{
		Attribute i = tagAttributes.front();
		if (!i.hasEquals && !i.hasName && !i.hasValue)
		{
			tagAttributes.pop_back();
		}
	}

	return tagAttributes;
}

std::string XMLTagParser::getIsolatedAttributesFrom(std::string tokenToIsolateAttributesFrom)
{
	int byPassesTagNameInToken = tokenToIsolateAttributesFrom.find_first_of(' ');
	if (byPassesTagNameInToken == -1)
	{
		return "";
	}
	else
	{
		if (tokenToIsolateAttributesFrom[tokenToIsolateAttributesFrom.length() - 2] == '?')
		{
			return tokenToIsolateAttributesFrom.substr(
				byPassesTagNameInToken + 1, (tokenToIsolateAttributesFrom.length() - byPassesTagNameInToken) - 3);
		}
		else
		{
			return tokenToIsolateAttributesFrom.substr(
				byPassesTagNameInToken + 1, (tokenToIsolateAttributesFrom.length() - 2) - byPassesTagNameInToken);
		}
	}
}

std::vector<Attribute> XMLTagParser::findOutAttributesFrom(std::string tokenToReadAttributesFrom)
{
	std::string isolatedAttributes = getIsolatedAttributesFrom(tokenToReadAttributesFrom);
	std::vector<Attribute> parsedAttributes = parseAttributesAsObjectsFrom(isolatedAttributes);
	return parsedAttributes;
}

Tag* XMLTagParser::getTagAsObject(Lexer::tokenLineNumPairs token)
{
	Tag* newTag = tagSupplier.getTagFor("xml");
	newTag->updateRequiresClosing(findOutIfNeedsClosingTag(token.token));
	newTag->updateIsClosing(findOutIfItIsAClosingTag(token.token));
	newTag->setTagName(findOutTagName(token.token));
	newTag->setLineNumber(token.lineNumber);
	newTag->setAttributes(findOutAttributesFrom(token.token));
	return newTag;
}

std::priority_queue<int, std::vector<int>, std::greater<int>> XMLTagParser::sort(
	std::vector<int> openingTags, std::vector<int> closingTags)
{
	std::priority_queue<int, std::vector<int>, std::greater<int>> sortedLocations;
	placeCollectionIntoSortedContainer(&sortedLocations, openingTags);
	placeCollectionIntoSortedContainer(&sortedLocations, closingTags);
	return sortedLocations;
}

void XMLTagParser::placeCollectionIntoSortedContainer(
	std::priority_queue<int, std::vector<int>, std::greater<int>>* goingToContainer, 
	std::vector<int> goingFromContainer)
{
	while (!goingFromContainer.empty())
	{
		goingToContainer->emplace(goingFromContainer.back());
		goingFromContainer.pop_back();
	}
}
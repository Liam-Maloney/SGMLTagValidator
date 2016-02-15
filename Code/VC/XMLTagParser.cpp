#include "stdafx.h"
#include "XMLTagParser.h"

XMLTagParser::XMLTagParser()
{

}

std::vector<Tag*> XMLTagParser::parseTagsFrom(std::string fileToParseFrom)
{
	IO* source = ioHandle.getGenericIoHandle(fileToParseFrom);
	return getTagsAsVectorParsedFrom(source->getContent());
}

std::vector<Tag*> XMLTagParser::getTagsAsVectorParsedFrom(std::string source)
{
	std::vector<tokenLineNumPairs> tokenizedTags = readTokenizedTags(source);
	std::vector<Tag*> parsedTags = formTagsAsObjects(tokenizedTags);
	return parsedTags;
}

std::vector<XMLTagParser::tokenLineNumPairs> XMLTagParser::readTokenizedTags(std::string inSource)
{
	std::vector<int> sortedDelimitersOfTokens = getSortedDelimitersForTokens(inSource);
	removeDelimitersBetweenQuotesIn(&sortedDelimitersOfTokens, inSource);
	std::vector<tokenLineNumPairs> tokens = tokenizeAccoringTo(sortedDelimitersOfTokens, inSource);
	return tokens;
}

std::vector<int> XMLTagParser::getSortedDelimitersForTokens(
	std::string inSource)
{
	std::vector<int> locationsOfOpenTag = findLocationsOf('<', inSource);
	std::vector<int> locationsOfClosingTag = findLocationsOf('>', inSource);
	std::vector<int> sortedDelimiters = locationsOfClosingTag;
	sortedDelimiters.insert(sortedDelimiters.begin(), locationsOfOpenTag.begin(),
		locationsOfOpenTag.end());
	std::sort(sortedDelimiters.begin(), sortedDelimiters.end());
	return sortedDelimiters;
}

void XMLTagParser::removeDelimitersBetweenQuotesIn(std::vector<int> *sortedDelimitersOfTokens,
	std::string inSource)
{
	int upperQuote;
	int lowerQuote;

	std::vector<int> locationsToRemoveDelimitersBetween = findLocationsOf('"', inSource);
	while (!locationsToRemoveDelimitersBetween.empty())
	{
		upperQuote = locationsToRemoveDelimitersBetween.back();
		locationsToRemoveDelimitersBetween.pop_back();
		if (!locationsToRemoveDelimitersBetween.empty())
		{
			lowerQuote = locationsToRemoveDelimitersBetween.back();
			locationsToRemoveDelimitersBetween.pop_back();
		}
		if ((lowerQuote == (upperQuote - 1)) && (lowerQuote == 0))
		{
			return;
		}
		else
		{
			removeSortedDelimitersBetweenIndexOf(
				lowerQuote, upperQuote, sortedDelimitersOfTokens);
		}
	}
}

std::vector<XMLTagParser::tokenLineNumPairs> XMLTagParser::tokenizeAccoringTo(
	std::vector<int> sortedDelimitersOfTokens,
	std::string inSource)
{
	std::vector<tokenLineNumPairs> tokenBuildup;
	int atStartIndex = 0;
	int upToThisPoint = -1;

	std::vector<int> lineNumberIndexes = findLocationsOf('\n', inSource);
	int lineNum = 1;

	while (!sortedDelimitersOfTokens.empty())
	{
		progressToOpenTagSymbol(&sortedDelimitersOfTokens, inSource);
		atStartIndex = sortedDelimitersOfTokens.front();
		sortedDelimitersOfTokens.erase(sortedDelimitersOfTokens.begin());
		lineNum += updateLineNumber(&lineNumberIndexes, &sortedDelimitersOfTokens);
		progressToCloseTagSymbol(&sortedDelimitersOfTokens, inSource);
		upToThisPoint = sortedDelimitersOfTokens.front();
		sortedDelimitersOfTokens.erase(sortedDelimitersOfTokens.begin());
		tokenLineNumPairs newTokenLineNumPair = formTokenLineNumPair(atStartIndex,
			upToThisPoint, lineNum, inSource);
		tokenBuildup.emplace_back(newTokenLineNumPair);
	}
	return tokenBuildup;
}

std::vector<int> XMLTagParser::findLocationsOf(char whatImLookingFor, std::string insideThis)
{
	const int NO_MORE_LEFT = -1;
	std::vector<int> locations;
	int searcherLocation = insideThis.find_first_of(whatImLookingFor);

	while (searcherLocation != NO_MORE_LEFT)
	{
		locations.push_back(searcherLocation);
		searcherLocation = insideThis.find_first_of(whatImLookingFor, searcherLocation + 1);
	}
	return locations;
}

void XMLTagParser::removeSortedDelimitersBetweenIndexOf(int lowerQuote, int upperQuote,
	std::vector<int>* sortedDelimitersOfTokens)
{
	//reverse iteration as this does not invalidate vector container
	std::vector<int>::iterator i = sortedDelimitersOfTokens->end();
	i--;
	for (; i != sortedDelimitersOfTokens->begin(); i--)
	{
		if (*i > lowerQuote && *i < upperQuote)
		{
			//erase returns non-invalidated iterator
			i = sortedDelimitersOfTokens->erase(i);
		}
		else if (*i < lowerQuote)
		{
			return;
		}
	}

	//final loop
	if (*i > lowerQuote && *i < upperQuote)
	{
		//erase returns non-invalidated iterator
		i = sortedDelimitersOfTokens->erase(i);
	}
	else if (*i < lowerQuote)
	{
		return;
	}
}

void XMLTagParser::progressToOpenTagSymbol(std::vector<int>* sortedDelimitersOfTokens, std::string inSource)
{
	while (inSource[sortedDelimitersOfTokens->front()] != '<' && !sortedDelimitersOfTokens->empty())
	{
		//pop_front hack!
		sortedDelimitersOfTokens->erase(sortedDelimitersOfTokens->begin());
	}
}

void XMLTagParser::progressToCloseTagSymbol(std::vector<int>* sortedDelimitersOfTokens, std::string inSource)
{
	while ((inSource[sortedDelimitersOfTokens->front()] != '>' && inSource[sortedDelimitersOfTokens->front()] != '<') 
		&& !sortedDelimitersOfTokens->empty())
	{
		sortedDelimitersOfTokens->erase(sortedDelimitersOfTokens->begin());
	}
}

int XMLTagParser::updateLineNumber(std::vector<int>* lineNumberIndexes, std::vector<int>* sortedDelimitersOfTokens)
{
	int progressedByThisManyLineNums = 0;
	while (!lineNumberIndexes->empty() && lineNumberIndexes->front() < sortedDelimitersOfTokens->front())
	{
		lineNumberIndexes->erase(lineNumberIndexes->begin());
		progressedByThisManyLineNums++;
	}
	return progressedByThisManyLineNums;
}

XMLTagParser::tokenLineNumPairs XMLTagParser::formTokenLineNumPair(int from, int to, int line, std::string inSource)
{
	tokenLineNumPairs newTokenLineNumPair;
	newTokenLineNumPair.token = inSource.substr(from, (to + 1) - from);
	newTokenLineNumPair.lineNumber = line;
	return newTokenLineNumPair;
}

std::vector<Tag*> XMLTagParser::formTagsAsObjects(std::vector<XMLTagParser::tokenLineNumPairs> tokensToTurnInToTags)
{
	std::vector<Tag*> tagObjects;
	for each (tokenLineNumPairs token in tokensToTurnInToTags)
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
	std::vector<int> ignoreSpacesBetween = findLocationsOf('"', isolatedAttributes);
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

std::vector<std::string> removeRedundantEntries(std::vector<std::string> substrings)
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

bool notAlreadyPartOfAttribute(int index, std::vector<std::string> substrings)
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

bool rightOfThisIsValue(int index, std::vector<std::string> substrings)
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

bool leftOfThisIsName(int index, std::vector<std::string> substrings)
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

void concatRightValueToThisEquals(int indexOfEqualsToConcatTo, std::vector<std::string>* substrings)
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

void concatLeftNameToThisEquals(int indexOfEqualsToConcatTo, std::vector<std::string>* substrings)
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
	std::vector<int> indexOfSpaces = findLocationsOf(' ', *isolatedAttributes);
	std::vector<std::string> substrings;
	if (indexOfSpaces.empty())
	{
		substrings.emplace_back(*isolatedAttributes);
	}
	else
	{
		removeDelimitersBetweenQuotesIn(&indexOfSpaces, *isolatedAttributes);
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

Tag* XMLTagParser::getTagAsObject(XMLTagParser::tokenLineNumPairs token)
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
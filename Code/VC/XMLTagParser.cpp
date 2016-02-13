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
		removeSortedDelimitersBetweenIndexOf(lowerQuote, upperQuote, sortedDelimitersOfTokens);
	}
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

XMLTagParser::tokenLineNumPairs XMLTagParser::formTokenLineNumPair(int from, int to, int line, std::string inSource)
{
	tokenLineNumPairs newTokenLineNumPair;
	newTokenLineNumPair.token = inSource.substr(from, (to + 1) - from);
	newTokenLineNumPair.lineNumber = line;
	return newTokenLineNumPair;
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

Tag* XMLTagParser::getTagAsObject(XMLTagParser::tokenLineNumPairs token)
{
	Tag* newTag = tagSupplier.getTagFor("xml");
	newTag->updateRequiresClosing(findOutIfNeedsClosingTag(token.token));
	newTag->updateIsClosing(findOutIfItIsAClosingTag(token.token));
	newTag->setTagName(findOutTagName(token.token));
	newTag->setLineNumber(token.lineNumber);
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
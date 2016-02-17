#include "stdafx.h"
#include "Lexer.h"

std::vector<Lexer::tokenLineNumPairs> Lexer::readTokenizedTagsFrom(std::string source)
{
	std::vector<int> sortedDelimitersOfTokens = getSortedDelimitersForTokens(source);
	Utility::removeDelimitersBetweenQuotesIn(&sortedDelimitersOfTokens, source);
	std::vector<tokenLineNumPairs> tokens = tokenizeAccordingTo(sortedDelimitersOfTokens, source);
	return tokens;
}

std::vector<int> Lexer::getSortedDelimitersForTokens(std::string inSource)
{
	std::vector<int> locationsOfOpenTag = Utility::findLocationsOf('<', inSource);
	std::vector<int> locationsOfClosingTag = Utility::findLocationsOf('>', inSource);
	std::vector<int> sortedDelimiters = locationsOfClosingTag;
	sortedDelimiters.insert(sortedDelimiters.begin(), locationsOfOpenTag.begin(),
		locationsOfOpenTag.end());
	std::sort(sortedDelimiters.begin(), sortedDelimiters.end());
	return sortedDelimiters;
}

std::vector<Lexer::tokenLineNumPairs> Lexer::tokenizeAccordingTo(std::vector<int> sortedDelimitersOfTokens, std::string inSource)
{
	std::vector<tokenLineNumPairs> tokenBuildup;
	int atStartIndex = 0;
	int upToThisPoint = -1;

	std::vector<int> lineNumberIndexes = Utility::findLocationsOf('\n', inSource);
	int lineNum = 1;

	while (!sortedDelimitersOfTokens.empty())
	{
		progressToOpenTagSymbol(&sortedDelimitersOfTokens, inSource);
		if (!sortedDelimitersOfTokens.empty())
		{
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
	}
	return tokenBuildup;
}

void Lexer::progressToOpenTagSymbol(std::vector<int>* sortedDelimitersOfTokens, std::string inSource)
{
	while (!sortedDelimitersOfTokens->empty() && inSource[sortedDelimitersOfTokens->front()] != '<')
	{
		//pop_front hack!
		sortedDelimitersOfTokens->erase(sortedDelimitersOfTokens->begin());
	}
}

int Lexer::updateLineNumber(std::vector<int>* lineNumberIndexes, std::vector<int>* sortedDelimitersOfTokens)
{
	int progressedByThisManyLineNums = 0;
	while (!lineNumberIndexes->empty() && lineNumberIndexes->front() < sortedDelimitersOfTokens->front())
	{
		lineNumberIndexes->erase(lineNumberIndexes->begin());
		progressedByThisManyLineNums++;
	}
	return progressedByThisManyLineNums;
}

void Lexer::progressToCloseTagSymbol(std::vector<int>* sortedDelimitersOfTokens, std::string inSource)
{
	while ((inSource[sortedDelimitersOfTokens->front()] != '>' && inSource[sortedDelimitersOfTokens->front()] != '<')
		&& !sortedDelimitersOfTokens->empty())
	{
		sortedDelimitersOfTokens->erase(sortedDelimitersOfTokens->begin());
	}
}

Lexer::tokenLineNumPairs Lexer::formTokenLineNumPair(int from, int to, int line, std::string inSource)
{
	tokenLineNumPairs newTokenLineNumPair;
	newTokenLineNumPair.token = inSource.substr(from, (to + 1) - from);
	newTokenLineNumPair.lineNumber = line;
	return newTokenLineNumPair;
}
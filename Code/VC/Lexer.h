#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include "Utility.h"

class Lexer
{
	struct tokenLineNumPairs;

	std::vector<int> getSortedDelimitersForTokens(std::string inSource);
	std::vector<tokenLineNumPairs> tokenizeAccordingTo(std::vector<int> sortedDelimitersOfTokens, std::string inSource);
	void progressToOpenTagSymbol(std::vector<int>* sortedDelimitersOfTokens, std::string inSource);
	int updateLineNumber(std::vector<int>* lineNumberIndexes, std::vector<int>* sortedDelimitersOfTokens);
	void progressToCloseTagSymbol(std::vector<int>* sortedDelimitersOfTokens, std::string inSource);
	tokenLineNumPairs formTokenLineNumPair(int from, int to, int line, std::string inSource);

public:

	struct tokenLineNumPairs
	{
		std::string token;
		int lineNumber;
	};

	std::vector<tokenLineNumPairs> readTokenizedTagsFrom(std::string source);
};


#pragma once
#include <vector>

class Utility
{
public:

	static void removeDelimitersBetweenQuotesIn(std::vector<int> *sortedDelimitersOfTokens, std::string inSource);
	static std::vector<int> findLocationsOf(char whatImLookingFor, std::string insideThis);
	static void removeSortedDelimitersBetweenIndexOf(int lowerQuote, int upperQuote,
		std::vector<int>* sortedDelimitersOfTokens);

	Utility();
	~Utility();
};


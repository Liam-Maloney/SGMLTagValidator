#include "stdafx.h"
#include "Utility.h"

std::vector<int> Utility::findLocationsOf(char whatImLookingFor, std::string insideThis)
{
	const int NO_MORE_LEFT = -1;
	std::vector<int> locations;
	int searcherLocation = insideThis.find_first_of(whatImLookingFor);

	while (searcherLocation != NO_MORE_LEFT)
	{
		if (whatImLookingFor == '>')
		{
			if (searcherLocation > 2)
			{
				if (!(insideThis[searcherLocation] == '>' && insideThis[searcherLocation - 1] == '-' && insideThis[searcherLocation - 2] == '-'))
				{
					locations.push_back(searcherLocation);
					searcherLocation = insideThis.find_first_of(whatImLookingFor, searcherLocation + 1);
				}
				else
				{
					searcherLocation = insideThis.find_first_of(whatImLookingFor, searcherLocation + 1);
				}
			}
			else
			{
				locations.push_back(searcherLocation);
				searcherLocation = insideThis.find_first_of(whatImLookingFor, searcherLocation + 1);
			}
		}
		else
		{
			locations.push_back(searcherLocation);
			searcherLocation = insideThis.find_first_of(whatImLookingFor, searcherLocation + 1);
		}
	}
	return locations;
}

void Utility::removeDelimitersBetweenQuotesIn(std::vector<int> *sortedDelimitersOfTokens, std::string inSource)
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

void Utility::removeSortedDelimitersBetweenIndexOf(int lowerQuote, int upperQuote,
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

Utility::Utility()
{
}


Utility::~Utility()
{
}

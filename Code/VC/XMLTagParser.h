#pragma once
#include "TagParser.h"
#include <string>
#include <vector>
#include <queue>
#include <fstream>
#include "TagFactory.h"
#include "IOFactory.h"
#include <functional>
#include <queue>
#include <vector>
#include <ctype.h>

class XMLTagParser : public TagParser 
{
	TagFactory tagSupplier;
	IOFactory ioHandle;

	struct tokenLineNumPairs
	{
		std::string token;
		int lineNumber;
	};

	std::vector<tokenLineNumPairs> readTokenizedTags(std::string inSource);
	std::vector<tokenLineNumPairs> tokenizeAccoringTo(
		std::vector<int> sortedDelimitersOfTokens,
		std::string inSource);
	std::vector<int> getSortedDelimitersForTokens(std::string inSource);
	std::vector<Tag*> formTagsAsObjects(std::vector<tokenLineNumPairs>);
	std::priority_queue<int, std::vector<int>, std::greater<int>>
		sort(std::vector<int> openingTags, std::vector<int> closingTags);
	std::vector<int> findLocationsOf(char whatImLookingFor, std::string insideThis);
	void placeCollectionIntoSortedContainer(
		std::priority_queue<int, std::vector<int>, std::greater<int>>* goingToContainer,
		std::vector<int> goingFromContainer);
	void removeDelimitersBetweenQuotesIn(std::vector<int> *sortedDelimitersOfTokens,
		std::string inSource);
	void removeSortedDelimitersBetweenIndexOf(int lowerQuote, int upperQuote,
		std::vector<int> *sortedDelimitersOfTokens);
	Tag* getTagAsObject(tokenLineNumPairs token);
	void progressToCloseTagSymbol(std::vector<int>* sortedDelimitersOfTokens, std::string inSource);
	void progressToOpenTagSymbol(std::vector<int>* sortedDelimitersOfTokens, std::string inSource);
	int updateLineNumber(std::vector<int>* lineNumberIndexes, std::vector<int>* sortedDelimitersOfTokens);
	tokenLineNumPairs formTokenLineNumPair(int from, int to, int line, std::string inSource);
	bool findOutIfNeedsClosingTag(std::string token);
	bool findOutIfItIsAClosingTag(std::string token);
	std::string findOutTagName(std::string token);
	std::vector<Attribute> findOutAttributesFrom(std::string tokenToReadAttributesFrom);
	std::string XMLTagParser::getIsolatedAttributesFrom(std::string tokenToIsolateAttributesFrom);
	std::vector<Attribute> parseAttributesAsObjectsFrom(std::string isolatedAttributes);
	std::string getNextAttributeString(std::string isolatedAttributes);
	Attribute constructAttribute(std::string makeAttributeFromThis);
	std::string popFirstAttribute(std::string isolatedAttributes);
	int getEndIndexOfAttribute(std::string isolatedAttributes);
	std::string parseAttributeName(std::string fullAttribute);
	bool hasEquals(std::string fullAttribute);
	bool hasValue(std::string fullAttribute);
	std::string parseAttributeValue(std::string fullAttribute);
	std::vector<std::string> removeRepeatedSpacesIn(std::string* isolatedAttributes);
	std::vector<std::string> getAllSubstringsBetweenSpaces(std::vector<int> *indexOfSpaces,
		std::string* isolatedAttributes);
	std::vector<std::string> removeSpaceAroundNameValuePairs(std::vector<std::string> substrings);
	void removeSpacesIfPartOfNameValuePair(std::vector<std::string>& substrings,
		std::vector<std::string>::iterator& i);
	bool flankedByNameValuePair(std::vector<std::string>& substrings, 
		std::vector<std::string>::iterator& i);
	std::vector<int> getIndexOfEqualsInVector(std::vector<std::string>* substrings);

public:

	std::vector<Tag*> getTagsAsVectorParsedFrom(std::string source);
	std::vector<Tag*> parseTagsFrom(std::string fileToParseFrom);
	
	XMLTagParser();
	XMLTagParser(std::string);
	~XMLTagParser();
};
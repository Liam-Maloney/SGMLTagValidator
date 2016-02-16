#pragma once
#include <functional>
#include <queue>
#include "Lexer.h"
#include "Utility.h"
#include "Attribute.h"
#include "Tag.h"
#include "TagFactory.h"
#include "IOFactory.h"

class SGMLParser
{

protected:

	TagFactory tagSupplier;
	IOFactory ioHandle;

	bool leftOfThisIsName(int index, std::vector<std::string> substrings);
	std::vector<std::string> removeRedundantEntries(std::vector<std::string> substrings);
	bool notAlreadyPartOfAttribute(int indeax, std::vector<std::string> substrings);
	bool rightOfThisIsValue(int index, std::vector<std::string> substrings);
	void concatRightValueToThisEquals(int indexOfEqualsToConcatTo, std::vector<std::string>* substrings);
	void concatLeftNameToThisEquals(int indexOfEqualsToConcatTo, std::vector<std::string>* substrings);
	std::priority_queue<int, std::vector<int>, std::greater<int>> sort(std::vector<int> openingTags, std::vector<int> closingTags);
	void placeCollectionIntoSortedContainer(std::priority_queue<int, std::vector<int>, std::greater<int>>* goingToContainer, std::vector<int> goingFromContainer);
	bool findOutIfItIsAClosingTag(std::string token);
	std::string findOutTagName(std::string token);
	std::vector<Attribute> findOutAttributesFrom(std::string tokenToReadAttributesFrom);
	std::string getIsolatedAttributesFrom(std::string tokenToIsolateAttributesFrom);
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
	std::vector<std::string> getAllSubstringsBetweenSpaces(std::vector<int> *indexOfSpaces, std::string* isolatedAttributes);
	std::vector<std::string> removeSpaceAroundNameValuePairs(std::vector<std::string> substrings);
	std::vector<int> getIndexOfEqualsInVector(std::vector<std::string>* substrings);

public:
	SGMLParser();
	virtual ~SGMLParser();

};


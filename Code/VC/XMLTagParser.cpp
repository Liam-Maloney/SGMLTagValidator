#include "stdafx.h"
#include "XMLTagParser.h"

std::list<Tag*> XMLTagParser::getTagsAsListParsedFrom(IO* file)
{
	std::string unparsedText = file->getContent();
	return parseFromSource(unparsedText);
}

std::list<Tag*> XMLTagParser::parseTagsFrom(std::string fileToParseFrom)
{
	IO* source = ioHandle.getGenericIoHandle(fileToParseFrom);

	return getTagsAsListParsedFrom(source);
}

XMLTagParser::XMLTagParser()
{

}

struct XMLTagParser::simpleTag
{
	std::string token;
	int lineNumber;
	bool tagIsClosingTag;
	bool tagIsSelfClosingTag;
};

bool XMLTagParser::isValidSymbol(char current)
{
	switch (current)
	{
		case ' ':
		case '=':
		case '"':
			return true;
			break;
	}
	return false;
}

std::queue<XMLTagParser::queueEntry> XMLTagParser::getOrder(std::string source)
{
	std::queue<queueEntry> orderedInputSymbols;
	for (int i = 0; i < source.length(); i++)
	{
		char current = source[i];
		
		if (isValidSymbol(current))
		{
			queueEntry newEntry;
			newEntry.index = i;
			newEntry.symbol = source[i];
			orderedInputSymbols.push(newEntry);
		}
	}
	return orderedInputSymbols;
}

std::list<XMLTagParser::simpleTag> XMLTagParser::tokenizeTags(std::string source)
{
	std::list<XMLTagParser::simpleTag> tokens;
	int lineNumber = 1;
	std::string contentsOfTag = "";
	
	bool tagStarted = false;
	bool buildingTag = true;
	bool isAClosingTag = false;
	bool requiresAClosingTag = true;
	int countOfQuotes = 0;
	int i = source.find_first_of('<');
	int missingFirstOpenTag = source.find_first_of('>');

	if (missingFirstOpenTag < i)
	{
		if (missingFirstOpenTag != -1)
		{
			i = missingFirstOpenTag;
		}
	}

	if (i == -1)
	{
		i = source.find_first_of('>');
	}

	for (; i < source.length(); i++)
	{
		if (source[i] == '"')
		{
			countOfQuotes++;
			int indexOfNextQuote = source.find_first_of('"', i + 1);
			if (indexOfNextQuote == -1)
			{
				indexOfNextQuote = source.length() - 1;
			}
			int length = indexOfNextQuote - i;
			contentsOfTag += source.substr(i, length + 1);
			i = indexOfNextQuote;
			
		}
		else
		{
			if (source[i] == '\n')
			{
				lineNumber++;
			}

			if (source[i] == '<')
			{
				if (tagStarted)
				{
					//save the previous tag as it was not closed, this is a syntax error
					XMLTagParser::simpleTag nextTag;
					nextTag.token = contentsOfTag;
					nextTag.lineNumber = lineNumber;
					nextTag.tagIsClosingTag = isAClosingTag;
					nextTag.tagIsSelfClosingTag = !requiresAClosingTag;
					tokens.emplace_back(nextTag);
					contentsOfTag = "";
					requiresAClosingTag = true;
					isAClosingTag = false;
				}
				tagStarted = true;
				contentsOfTag += source[i];
				if (source[i + 1] == '/')
				{
					isAClosingTag = true;
					//TODO: REMOVE THE DUPLICATION IN LOGIC HERE, I SHOULD ONLY NEED ONE POTENTIALL!!!
					requiresAClosingTag = false;
					//TODO: REFACTOR; CONTINUE IS SKIPPING THE '/' CHARACTER AT THE START OF A CLOSING TAG
					continue;
				}
			}
			else if (tagStarted && source[i] != '>')
			{
				if (source[i] != '/')
				{
					contentsOfTag += source[i];
				}

				if (source[i - 1] == '/' || source[i - 1] == '?')
				{
					requiresAClosingTag = false;
				}
			}
			else if (source[i] == '>')
			{
				tagStarted = false;
				buildingTag = false;
				contentsOfTag += source[i];
				XMLTagParser::simpleTag nextTag;
				nextTag.token = contentsOfTag;
				nextTag.lineNumber = lineNumber;
				nextTag.tagIsClosingTag = isAClosingTag;
				//TODO: CHANGE THIS SO THAT I DON'T NEED TO DO THE NEGATION 
				//THERE IS A CONFLICT IN THE VARIABLE NAMES WHICH MAKES THE ASSINMENT AMBIGUOUS!
				nextTag.tagIsSelfClosingTag = !requiresAClosingTag;
				tokens.emplace_back(nextTag);
				contentsOfTag = "";
				requiresAClosingTag = true;
				isAClosingTag = false;
			}
		}
	}
	
	if (contentsOfTag.length() != 0 )//&& contentsOfTag[contentsOfTag.length() -1] != '>')
	{
		XMLTagParser::simpleTag nextTag;
		nextTag.token = contentsOfTag;
		nextTag.lineNumber = lineNumber;
		nextTag.tagIsClosingTag = isAClosingTag;
		//TODO: CHANGE THIS SO THAT I DON'T NEED TO DO THE NEGATION 
		//THERE IS A CONFLICT IN THE VARIABLE NAMES WHICH MAKES THE ASSINMENT AMBIGUOUS!
		nextTag.tagIsSelfClosingTag = !requiresAClosingTag;
		tokens.emplace_back(nextTag);
	}
	
	
	

	return tokens;
}

bool XMLTagParser::notAtEndOfName(char endOfTagDelimiter)
{
	if (endOfTagDelimiter == '>')
	{
		return false;
	}
	if (endOfTagDelimiter == ' ')
	{
		return false;
	}
	return true;
}

std::string XMLTagParser::readTagFullName(std::string fullTag)
{
	const int SKIP_FIRST_BRACKET = 1;
	std::string tagName = "";
	if (fullTag[0] == '>')
	{
		return ">";
	}
	else
	{
		for (int i = SKIP_FIRST_BRACKET; notAtEndOfName(fullTag[i]); i++)
		{
			tagName += fullTag[i];
		}
	}
	return tagName;
}


std::string XMLTagParser::removeExtraSpaces(std::string currentTag)
{
	std::string processedTag = "";
	bool insideQuotes = false;
	bool skipSpaces = false;
	for (int i = 0; i < currentTag.length(); i++)
	{
		if (insideQuotes)
		{
			if (currentTag[i] == '"')
			{
				insideQuotes = false;
				processedTag += '"';
			}
			else
			{
				processedTag += currentTag[i];
			}
		}
		else
		{
			if (currentTag[i] == ' ')
			{
				if (!skipSpaces)
				{
					processedTag += currentTag[i];
					skipSpaces = true;
				}
			}
			else if (currentTag[i] == '"')
			{
				insideQuotes = true;
				processedTag += currentTag[i];
			}
			else
			{
				skipSpaces = false;
				processedTag += currentTag[i];
			}
		}
	}
	return processedTag;
}

std::string XMLTagParser::removeSpacesAroundEquals(std::string tagWithRemovedSpaces)
{
	std::string processed = "";
	int i = 0;
	bool justParsedName = true;

	//skips the name of the tag
	while (tagWithRemovedSpaces[i] != ' ' && tagWithRemovedSpaces[i] != '=' && tagWithRemovedSpaces[i] != '"' && tagWithRemovedSpaces[i] != '>')
	{
		processed += tagWithRemovedSpaces[i];
		i++;
	}

	if (i != tagWithRemovedSpaces.length())
	{
		processed += ' ';
	}
	else if (tagWithRemovedSpaces[i] == '>')
	{
		processed += '>';
	}

	while (i < tagWithRemovedSpaces.length())
	{
		if (tagWithRemovedSpaces[i] == ' ')
		{
			if (tagWithRemovedSpaces[i + 1] == '=' || tagWithRemovedSpaces[i - 1] == '=')
			{

			}
			else
			{
				if (!justParsedName)
				{
					processed += tagWithRemovedSpaces[i];
				}
			}
			i++;
		}
		else
		{
			if (tagWithRemovedSpaces[i] == '"')
			{
				int substringTo = tagWithRemovedSpaces.find_first_of('"', i + 1);

				if (substringTo == -1) //unclosed quotes error
				{
					substringTo = tagWithRemovedSpaces.length() - 1;
				}

				std::string contentsOfQuotes = tagWithRemovedSpaces.substr(i, (substringTo - i) + 1);
				processed += contentsOfQuotes;
				i = (tagWithRemovedSpaces.find('"', i + 1));
				
				if (i == -1)
				{
					return processed;
				}

				i++;

			}
			else
			{
				processed += tagWithRemovedSpaces[i];
				i++;
			}
		}
		justParsedName = false;
	}
	
	return processed;
}

//todo move these inside the class itself instread of being external functions

bool encounteredASpace(char testChar)
{
	return (testChar == ' ') ? true : false;
}

bool encounteredAQuote(char testChar)
{
	return (testChar == '"') ? true : false;
}

bool encounteredAnEquals(char testChar)
{
	return (testChar == '=') ? true : false;
}

void addAttributeEntryParsedFrom(int startParse, int endParse, std::string processedAttr, std::list<std::string>* listOfAttributes)
{
	std::string tagWithWhiteSpace = processedAttr.substr(startParse, endParse - startParse + 1);
	if (tagWithWhiteSpace == " ")
	{
		return;
	}
	if (tagWithWhiteSpace[0] == ' ')
	{
		int startIndex = tagWithWhiteSpace.find_first_not_of(' ');
		std::string tagWithoutWhitespace = tagWithWhiteSpace.substr(startIndex, tagWithoutWhitespace.length() - startIndex);

		listOfAttributes->emplace_back(tagWithoutWhitespace);
	}
	else
	{
		listOfAttributes->emplace_back(tagWithWhiteSpace);
	}
}

XMLTagParser::queueEntry XMLTagParser::getNextQuoteFrom(std::queue<queueEntry>* orderedSyntax)
{
	while (orderedSyntax->front().symbol != '"')
	{
		orderedSyntax->pop();
	}

	queueEntry nextQuote = orderedSyntax->front();
	orderedSyntax->pop();
	return nextQuote;
}

//TODO:RENAME
std::list<std::string> XMLTagParser::finallyGetAttributes(std::string processedAttr)
{
	std::list<std::string> listOfAttributes;
	std::string currentAttr = "";
	std::queue<queueEntry> orderedSyntax = getOrder(processedAttr);

	//discard first space, as it is our starting point
	
	bool alreadyGotSpace = true; //needs to be true as I have encounter a space implicityly at the begging of the algorithms
	bool alreadyGotQuote = false;
	bool alreadyGotEquals = false;
	int beginningOfParse = 0;
	int toEndOfParse = 0;
	int countOfQuotes = 0;

	for (int i = 0; i < processedAttr.length(); i++)
	{
		if (processedAttr[i] == '"')
		{
			countOfQuotes++;
		}
	}

	if (orderedSyntax.empty())
	{
		return listOfAttributes;
	}

	queueEntry skipTagName = orderedSyntax.front();
	orderedSyntax.pop();

	beginningOfParse = skipTagName.index;

	bool skippedLoop = true;

	while (!orderedSyntax.empty())
	{
		skippedLoop = false;
		queueEntry inCurrent = orderedSyntax.front();
		orderedSyntax.pop();

		if (!orderedSyntax.empty())
		{

			if (encounteredASpace(inCurrent.symbol)) //think inside here is ok
			{
				if (alreadyGotSpace || alreadyGotEquals){
					//then this tag has an error so i should add what exists in the current and restart
					toEndOfParse = inCurrent.index;
					addAttributeEntryParsedFrom(beginningOfParse, toEndOfParse-1, processedAttr, &listOfAttributes);
					alreadyGotEquals = false;
					beginningOfParse = inCurrent.index;
					alreadyGotSpace = false;
				}
				else
				{
					beginningOfParse = inCurrent.index;
					alreadyGotSpace = true;
				}
			}
			else if (encounteredAnEquals(inCurrent.symbol))
			{
				if (alreadyGotSpace)
				{
					//this is the happy path, we now expect a quote next
					alreadyGotEquals = true;
					toEndOfParse = inCurrent.index;
				}
				else if (alreadyGotEquals)
				{
					//this means we have encountered another error
					toEndOfParse = inCurrent.index;
					addAttributeEntryParsedFrom(beginningOfParse, toEndOfParse, processedAttr, &listOfAttributes);
					alreadyGotEquals = false;
					alreadyGotSpace = false;
					beginningOfParse = inCurrent.index;
				}
				else
				{
					//this case means that the attribute that attribute we start here will not have an attribute name
					beginningOfParse = inCurrent.index;
					if (processedAttr[toEndOfParse] == ' ')
					{
						std::swap(toEndOfParse, beginningOfParse);
					}
				}
			}
			else if (encounteredAQuote(inCurrent.symbol))
			{
				//if (alreadyGotEquals && alreadyGotSpace)
				//{
				inCurrent = getNextQuoteFrom(&orderedSyntax);
				toEndOfParse = inCurrent.index;
				addAttributeEntryParsedFrom(beginningOfParse, toEndOfParse, processedAttr, &listOfAttributes);
				alreadyGotEquals = false;
				alreadyGotQuote = false;
				beginningOfParse = ++toEndOfParse; // move each past the previously parsed attribute
				alreadyGotSpace = false;
			}
		}
	}

	if ((toEndOfParse != processedAttr.length()) && (!alreadyGotSpace))
	{
		addAttributeEntryParsedFrom(toEndOfParse, (processedAttr.length() - 2), processedAttr, &listOfAttributes);
	}

	if (countOfQuotes % 2 != 0) //must be a missing quote
	{
		std::string noClosingQuotesString = processedAttr.substr(beginningOfParse, processedAttr.length() - beginningOfParse);
		if (noClosingQuotesString[0] == ' ')
		{
			int startIndex = noClosingQuotesString.find_first_not_of(' ');
			std::string tagWithoutWhitespace = noClosingQuotesString.substr(startIndex, tagWithoutWhitespace.length() - startIndex);
			listOfAttributes.emplace_back(tagWithoutWhitespace);
		}
	}

	return listOfAttributes;
}
//TODO: Refactor

std::list<std::string> XMLTagParser::readTagAttributes(std::string currentTag)
{
	std::string tagWithRemovedSpaces = removeExtraSpaces(currentTag);
	std::string minValidString = removeSpacesAroundEquals(tagWithRemovedSpaces);
	std::list<std::string> attributes = finallyGetAttributes(minValidString);
	return attributes;
}

Tag* XMLTagParser::getNewTagWithAttributesSet(std::string tagName, std::list<std::string> attributes, int lineNumber)
{
	Tag* tagBuildUp = tagSupplier.getTagFor("xml");
	tagBuildUp->setTagName(tagName);
	tagBuildUp->setAttributes(attributes);
	tagBuildUp->setLineNumber(lineNumber);
	
	//TODO: REFACTOR

	if (tagName.length() != 0 && tagName[1] == '/')
	{
		tagBuildUp->updateIsClosing(true);
	}
	else
	{
		tagBuildUp->updateIsClosing(false);
	}
	//TODO: NEED TO DO SOME PROCESSING TO FIGURE OUT IF THE TAG HAS A SELF CLOSE

	return tagBuildUp;
}

std::list<Tag*> XMLTagParser::formTagsAsObjects(std::list<XMLTagParser::simpleTag> fullTags)
{
	std::list<Tag*> tagDSBuilder;
	while (!fullTags.empty())
	{
		XMLTagParser::simpleTag nextTag = fullTags.front();
		fullTags.pop_front();
		std::string tagName = readTagFullName(nextTag.token);
		std::list<std::string> attributes = readTagAttributes(nextTag.token);
		Tag* xmlTag = getNewTagWithAttributesSet(tagName, attributes, nextTag.lineNumber);
		//TODO: REFACTOR THIS TO INCLUDE IT IN THE LINE ABOVE
		if (nextTag.tagIsClosingTag)
		{
			xmlTag->updateIsClosing(true);
		}
		else
		{
			xmlTag->updateIsClosing(false);
		}
		
		if (nextTag.tagIsSelfClosingTag)
		{
			xmlTag->updateRequiresClosing(false);
		}
		else
		{
			xmlTag->updateRequiresClosing(true);
		}

		tagDSBuilder.emplace_back(xmlTag);
	}
	return tagDSBuilder;
}

std::list<Tag*> XMLTagParser::parseFromSource(std::string source)
{
	std::list<XMLTagParser::simpleTag> fullTags = tokenizeTags(source);
	std::list<Tag*> parsedTags = formTagsAsObjects(fullTags);
	return parsedTags;
}
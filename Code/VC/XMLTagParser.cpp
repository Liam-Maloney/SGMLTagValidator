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
	if ((current >= 'A' && current <= 'Z') || (current >= 'a' && current <= 'z'))
	{
		return true;
	}

	switch (current)
	{
		case '<':
		case '>':
		case ' ':
		case '=':
		case '"':
			return true;
			break;
	}
}

std::queue<int> XMLTagParser::getOrder(std::string source)
{
	std::queue<int> orderedInputSymbols;
	for (int i = 0; i < source.length(); i++)
	{
		char current = source[i];
		
		if (isValidSymbol(current))
		{

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
	
	if (contentsOfTag.length() != 0 && contentsOfTag[contentsOfTag.length() -1] != '>')
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

				std::string contentsOfQuotes = tagWithRemovedSpaces.substr(i, (substringTo - i) + 1);
				processed += contentsOfQuotes;
				i = (tagWithRemovedSpaces.find('"', i + 1));
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

//TODO:RENAME
std::list<std::string> XMLTagParser::finallyGetAttributes(std::string processedAttr)
{
	std::list<std::string> listOfAttributes;
	std::string currentAttr = "";
	bool notPastNameTag = true;
	bool insideQuotes = false;
	for (int i = 0; i < processedAttr.length(); i++)
	{
		if (notPastNameTag)
		{
			if (processedAttr[i] == ' ' || processedAttr[i] == '"' || processedAttr[i] == '=')
				notPastNameTag = false;
		}
		else
		{
			if (insideQuotes)
			{
				currentAttr += processedAttr[i];
				if (processedAttr[i] == '"')
					insideQuotes = false;
			}
			else
			{
				if (processedAttr[i] == ' ' || i == processedAttr.length()-1)
				{
					listOfAttributes.emplace_back(currentAttr);
					currentAttr = "";
				}
				else if (processedAttr[i] == '"'){
					insideQuotes = true;
					currentAttr += processedAttr[i];
				}
				else{
					currentAttr += processedAttr[i];
				}
					
			}
			
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
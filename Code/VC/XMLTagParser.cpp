#include "stdafx.h"
#include "XMLTagParser.h"

std::list<Tag*> XMLTagParser::getTagsAsListParsedFrom(FileParser& file)
{
	std::string unparsedText = file.getContent();
	return parseFromSource(unparsedText);
}

std::list<Tag*> XMLTagParser::parseTagsFrom(std::string fileToParseFrom)
{
	FileParser sourceFile(fileToParseFrom);
	return getTagsAsListParsedFrom(sourceFile);
}

XMLTagParser::XMLTagParser()
{

}

struct XMLTagParser::simpleTag
{
	std::string token;
	int lineNumber;
};

std::list<XMLTagParser::simpleTag> tokenizeTags(std::string source)
{
	std::list<XMLTagParser::simpleTag> tokens;
	int lineNumber = 1;
	std::string contentsOfTag = "";
	bool tagStarted = false;
	bool buildingTag = true;

	//TODO: FIX PARSING OF TAGS WITH ONE ATTRIBUTE

	for (unsigned int i = 0; i < source.length(); i++)
	{
		if (source[i] == '\n')
		{
			lineNumber++;
		}

		if (source[i] == '<')
		{
			tagStarted = true;
			contentsOfTag += source[i];
		}
		else if (tagStarted && source[i] != '>')
		{
			contentsOfTag += source[i];
		}
		else if (source[i] == '>')
		{
			tagStarted = false;
			buildingTag = false;
			contentsOfTag += source[i];
			XMLTagParser::simpleTag nextTag;
			nextTag.token = contentsOfTag;
			nextTag.lineNumber = lineNumber;
			tokens.emplace_back(nextTag);
			contentsOfTag = "";
		}
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
	for (int i = SKIP_FIRST_BRACKET; notAtEndOfName(fullTag[i]); i++)
	{
		tagName += fullTag[i];
	}
	return tagName;
}

//TODO: Refactor

std::list<std::string> XMLTagParser::readTagAttributes(std::string currentTag)
{
	std::list<std::string> attributes;
	std::string attributeBuilder = "";
	bool firstSpaceNotEncountered = true;
	bool skipConsecutiveSpacesTag = false;
	bool notParsingInsideQuotes = true;
	const int CHOP_LAST_BRACKET = 1;

	for (int i = 0; i < currentTag.length() - CHOP_LAST_BRACKET; i++)
	{
		if (firstSpaceNotEncountered)
		{
			if (currentTag[i] == ' ')
			{
				firstSpaceNotEncountered = false;
			}
		}
		else if (currentTag[i] == '"')
		{
			if (notParsingInsideQuotes)
			{
				notParsingInsideQuotes = false;
				attributeBuilder += currentTag[i];
			}
			else
			{
				notParsingInsideQuotes = true;
				attributeBuilder += currentTag[i];
			}
		}
		else if (currentTag[i] == ' ' && notParsingInsideQuotes)
		{
			if (skipConsecutiveSpacesTag == true)
			{

			}
			else
			{
				skipConsecutiveSpacesTag = true;
				attributes.emplace_back(attributeBuilder);
				attributeBuilder = "";
			}
		}
		else
		{
			skipConsecutiveSpacesTag = false;
			attributeBuilder += currentTag[i];
		}
	}
	attributes.emplace_back(attributeBuilder);
	return attributes;
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
		tagDSBuilder.emplace_back(new Tag(tagName, attributes, nextTag.lineNumber));
	}
	return tagDSBuilder;
}

std::list<Tag*> XMLTagParser::parseFromSource(std::string source)
{
	std::list<XMLTagParser::simpleTag> fullTags = tokenizeTags(source);
	std::list<Tag*> parsedTags = formTagsAsObjects(fullTags);
	return parsedTags;
}
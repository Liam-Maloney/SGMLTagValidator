#include "stdafx.h"
#include "XMLTagParser.h"

std::list<Tag> XMLTagParser::getTagsAsListParsedFrom(FileParser& file)
{
	std::string unparsedText;
	std::list<Tag> parsedTagsFromFile;
	unparsedText = file.getContent();
	parsedTagsFromFile.splice(parsedTagsFromFile.end(), nextTagsInFile(unparsedText));
	return parsedTagsFromFile;
}

std::list<Tag> XMLTagParser::parseTagsFrom(std::string fileToParseFrom)
{
	FileParser sourceFile(fileToParseFrom);
	return getTagsAsListParsedFrom(sourceFile);
}

XMLTagParser::XMLTagParser()
{

}

std::list<Tag> XMLTagParser::nextTagsInFile(std::string currentContent)
{
	std::list<Tag> parsedTags;
	std::list<std::string> attr;
	std::string contentsOfTag = "";
	bool tagStarted = false;
	bool buildingTag = true;

	for (unsigned int i = 0; i < currentContent.length(); i++)
	{
		if (currentContent[i] == '<')
		{
			tagStarted = true;
			contentsOfTag += currentContent[i];
		}
		else if (tagStarted && currentContent[i] != '>')
		{
			contentsOfTag += currentContent[i];
		}
		else if (currentContent[i] == '>')
		{
			tagStarted = false;
			buildingTag = false;
			contentsOfTag += currentContent[i];
			parsedTags.emplace_back(Tag(contentsOfTag, attr));
		}
	}
	return parsedTags;
	/*
	char nextChar;
	bool tagStarted = false;
	bool buildingTag = true;

	while (notAtEndOfInputFile() && buildingTag)
	{
		fileHandle.get(nextChar);
		if (nextChar == '<')
		{
			tagStarted = true;
			contentsOfTag += nextChar;
		}
		else if (tagStarted && nextChar != '>')
		{
			contentsOfTag += nextChar;
		}
		else if (nextChar == '>')
		{
			tagStarted = false;
			buildingTag = false;
			contentsOfTag += nextChar;
		}
	}

	return Tag(contentsOfTag, attr);
	*/
}
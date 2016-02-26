#include "stdafx.h"
#include "FileParser.h"

FileParser::FileParser(std::string fileToParseFrom)
{
	fileHandle.open(fileToParseFrom);
	if (!(fileHandle.is_open()))
	{
		throw std::runtime_error("Error Opening File Stream to " + 
			fileToParseFrom + ".  Are you sure the file exists?\n\n");
	}
}

bool FileParser::hasMoreToParse()
{
	return (fileHandle.eof()) ? false : true;
}

std::string FileParser::readFile()
{
	std::string fullContent = "";
	char nextChar;

	fileHandle.get(nextChar);
	while (hasMoreToParse())
	{
		fullContent += nextChar;
		fileHandle.get(nextChar);
	}
	fileHandle.close();
	return fullContent;
}

std::vector<int> FileParser::findOpenComments(std::string fullContent)
{
	std::vector<int> startLocations;
	for (int i = 0; i < fullContent.length() - 1; i++)
	{
		if ((fullContent.length() - i) >= 4)
		{
			if (fullContent[i] == '<' && fullContent[i + 1] == '!' && fullContent[i + 2] == '-' && fullContent[i + 3] == '-')
			{
				startLocations.emplace_back(i);
			}
		}
	}
	return startLocations;
}

std::vector<int> FileParser::findClosingComments(std::string fullContent)
{
	std::vector<int> endLocations;
	for (int i = 0; i < fullContent.length() - 1; i++)
	{
		if ((fullContent.length() - i) >= 3)
		{
			if (fullContent[i] == '-' && fullContent[i + 1] == '-' && fullContent[i + 2] == '>')
			{
				endLocations.emplace_back(i + 2);
			}
		}
	}
	return endLocations;
}

std::string FileParser::removeCommentedSectionsFromSource(
	std::vector<int> openCommentLocations, std::vector<int> closeCommentLocations, 
	std::string fullContent)
{
	int front = -1;
	int back = -1;
	std::string contentWithCommentsRemoved = "";

	if (openCommentLocations.empty())
	{
		return fullContent;
	}
	else
	{
		front = openCommentLocations.front();
		contentWithCommentsRemoved += fullContent.substr(0, front);
		openCommentLocations.erase(openCommentLocations.begin());
	}

	while (!closeCommentLocations.empty() && !openCommentLocations.empty())
	{
		front = openCommentLocations.front();
		back = closeCommentLocations.front();
		contentWithCommentsRemoved += fullContent.substr(back, front - back);
		openCommentLocations.erase(openCommentLocations.begin());
		closeCommentLocations.erase(closeCommentLocations.begin());
	}

	if (openCommentLocations.empty() && !closeCommentLocations.empty())
	{
		back = closeCommentLocations.front();
		contentWithCommentsRemoved += fullContent.substr(
			back + 1, fullContent.length() - back);
	} 
	else if (closeCommentLocations.empty() && !openCommentLocations.empty())
	{
		contentWithCommentsRemoved += fullContent.substr(back);
	}
	else
	{
		return contentWithCommentsRemoved;
	}
	return contentWithCommentsRemoved;
}

std::string FileParser::getContent()
{
	std::string fullContent = readFile();
	std::vector<int> openCommentLocations = findOpenComments(fullContent);
	std::vector<int> closeCommentLocations = findClosingComments(fullContent);
	fullContent = removeCommentedSectionsFromSource(openCommentLocations, closeCommentLocations, fullContent);
	return fullContent;
}

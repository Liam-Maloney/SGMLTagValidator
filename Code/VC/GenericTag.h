#pragma once
#include <string>
#include <vector>
#include "Attribute.h"
#include <vector>
#include "Tag.h"

class GenericTag : public Tag
{

protected:

	bool closing;
	bool reqClosing;

	std::string tagName;
	std::vector<Attribute> attributes;
	int lineNumber = 0;
	void updateIsClosing(bool isClosing);
	void updateRequiresClosing(bool requiresClosing);
	bool isClosing();
	bool requiresClosing();
	std::vector<Attribute> getAttributes();
	std::string getTagName();
	int getLineNumber();
	void setAttributes(std::vector<Attribute> attributes);
	void setTagName(std::string tagName);
	void setLineNumber(int lineNumber);
};


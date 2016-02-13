#pragma once
#include "GenericTag.h"
#include "Tag.h"

class XMLTag : public Tag, public GenericTag
{

public:

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


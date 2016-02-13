#pragma once
#include "stdafx.h"
#include <string>
#include <vector>
#include "Attribute.h"

class Tag
{

public:

	virtual bool isClosing() = 0;
	virtual bool requiresClosing() = 0;
	virtual std::vector<Attribute> getAttributes() = 0;
	virtual std::string getTagName() = 0;
	virtual int getLineNumber() = 0;
	virtual void setAttributes(std::vector<Attribute> attributes) = 0;
	virtual void setTagName(std::string tagName) = 0;
	virtual void setLineNumber(int lineNumber) = 0;
	virtual void updateIsClosing(bool isClosing) = 0;
	virtual void updateRequiresClosing(bool requiresClosing) = 0;
};


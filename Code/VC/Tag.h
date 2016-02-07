#pragma once
#include "stdafx.h"
#include <string>
#include <list>

class Tag
{

public:

	virtual bool isClosing() = 0;
	virtual bool requiresClosing() = 0;
	virtual std::list<std::string> getAttributes() = 0;
	virtual std::string getTagName() = 0;
	virtual int getLineNumber() = 0;
	virtual void setAttributes(std::list<std::string> attributes) = 0;
	virtual void setTagName(std::string tagName) = 0;
	virtual void setLineNumber(int lineNumber) = 0;
	virtual void updateIsClosing(bool isClosing) = 0;
	virtual void updateRequiresClosing(bool requiresClosing) = 0;
};


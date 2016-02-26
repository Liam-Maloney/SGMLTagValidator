#include "stdafx.h"
#include "Test.h"
#include "ParserFactory.h"

//--------------- Parser system tests ---------------//

class SimulatedIO : public IO
{
	std::string simulatedSourceCode;
public:
	std::string getContent(){ return simulatedSourceCode; }
	SimulatedIO(std::string simulatedSourceCode)
	{
		this->simulatedSourceCode = simulatedSourceCode;
	}
};

class XMLTagParserTest{
public:
	ParserFactory parserSupplier;
	TagParserInterface* classUnderTest = parserSupplier.
		createParserFor("xml");
	IO* simulatedInputHandle;
};

struct XMLTagNameTestParams
{
	std::string simulatedInput;
	std::string expectedName;
};

class TagNameTests : public XMLTagParserTest, 
	public testing::Test, 
	public testing::WithParamInterface<XMLTagNameTestParams>{
public:
	XMLTagNameTestParams parameters = GetParam();
};

TEST_P(TagNameTests, TestContentsOfNameParse)
{

	SimulatedIO input(parameters.simulatedInput);
	simulatedInputHandle = &input;
	std::vector<Tag*> resultsOfTest = classUnderTest->
		getTagsAsVectorParsedFrom(simulatedInputHandle);
	std::string parsedResult = resultsOfTest.front()->getTagName();
	ASSERT_EQ(parameters.expectedName, parsedResult);
}

INSTANTIATE_TEST_CASE_P(default, TagNameTests,
	testing::Values(
	XMLTagNameTestParams{ "< /test>  > > xyz", "" },
	XMLTagNameTestParams{ "</test>>>xyz", "test" },
	XMLTagNameTestParams{ "</test this=>", "test" },
	XMLTagNameTestParams{ "<test this=>", "test" },
	XMLTagNameTestParams{ "< /test>", "" },
	XMLTagNameTestParams{ "</test>", "test" },
	XMLTagNameTestParams{ "<>", "" },
	XMLTagNameTestParams{ "<t >", "t" },
	XMLTagNameTestParams{ "<test with=\"attributes\">", "test" },
	XMLTagNameTestParams{ "<test    >", "test" },
	XMLTagNameTestParams{ "< test>", "" },
	XMLTagNameTestParams{ "<test >", "test" },
	XMLTagNameTestParams{"<test>", "test"}
));

//-------------------ATTRIBUTE NAME PARSE TESTS-----------------//

struct XMLAttributePareseParams
{
	std::string simulatedInput;
	std::string expectedAttributeName;
};

class AttributeNameTests : public XMLTagParserTest, 
	public testing::Test, 
	public testing::WithParamInterface<XMLAttributePareseParams>{
public:
	XMLAttributePareseParams parameters = GetParam();
};

TEST_P(AttributeNameTests, TestNameOfAttributeParse)
{

	SimulatedIO input(parameters.simulatedInput);
	simulatedInputHandle = &input;
	std::vector<Tag*> resultsOfTest = classUnderTest->
		getTagsAsVectorParsedFrom(simulatedInputHandle);
	std::string parsedResult = resultsOfTest.front()->
		getAttributes().front().name;
	ASSERT_EQ(parameters.expectedAttributeName, parsedResult);
}

INSTANTIATE_TEST_CASE_P(default, AttributeNameTests,
	testing::Values(
	XMLAttributePareseParams{ "<t t >", "t" },
	XMLAttributePareseParams{ "<t t>", "t" },
	XMLAttributePareseParams{ "</test this>  > > xyz", "this" },
	XMLAttributePareseParams{ "< /test>  > > xyz", "/test" },
	XMLAttributePareseParams{ "<test this=\"is\">", "this" },
	XMLAttributePareseParams{ "< this=\"is\">", "this" }
));

//-------------------ATTRIBUTE VALUE PARSE TESTS-----------------//

struct XMLAttributeValuePareseParams
{
	std::string simulatedInput;
	std::string expectedAttributeValue;
};

class AttributeValueTests : public XMLTagParserTest, 
	public testing::Test, 
	public testing::WithParamInterface<XMLAttributeValuePareseParams>{
public:
	XMLAttributeValuePareseParams parameters = GetParam();
};

TEST_P(AttributeValueTests, TestNameOfAttributeParse)
{

	SimulatedIO input(parameters.simulatedInput);
	simulatedInputHandle = &input;
	std::vector<Tag*> resultsOfTest = classUnderTest->
		getTagsAsVectorParsedFrom(simulatedInputHandle);
	std::string parsedResult = resultsOfTest.front()->
		getAttributes().front().value;
	if (parsedResult != "")
	{
		parsedResult = parsedResult.substr(1, parsedResult.length() - 2);
	}
	ASSERT_EQ(parameters.expectedAttributeValue, parsedResult);
}

INSTANTIATE_TEST_CASE_P(default, AttributeValueTests,
	testing::Values(
	XMLAttributeValuePareseParams{ 
	"<t t=\" <this is not a tag!>  \" >", " <this is not a tag!>  " },
	XMLAttributeValuePareseParams{ 
		"<t t=\" preservedSpaces  \" >", " preservedSpaces  " },
	XMLAttributeValuePareseParams{ "<t t >", "" },
	XMLAttributeValuePareseParams{ "< this=\"is\">", "is" }
));
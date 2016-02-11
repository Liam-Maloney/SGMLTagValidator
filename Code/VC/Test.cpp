#include "stdafx.h"
#include "Test.h"

//--------------- FILE IO TESTS ---------------//

class SimulatedIO : public IO
{
	/* 
		Class inherits from IO to allow for simulated input  
		to the XMLTagParser during test case
	*/
	std::string simulatedSourceCode;
public:
	std::string getContent(){ return simulatedSourceCode; }
	SimulatedIO(std::string simulatedSourceCode)
	{
		this->simulatedSourceCode = simulatedSourceCode;
	}
};

struct XMLTagCountParams
{
	std::string simulatedInput;
	int expectedNumOfTags;
};

class XMLTagParserTest : public testing::Test, public testing::WithParamInterface<XMLTagCountParams>{
public:
	XMLTagParser* ioParseTestInstance; //class under test
	XMLTagCountParams parameters = GetParam();
	IO* simulatedInputHandle;
};

TEST_P(XMLTagParserTest, CountOfTagsParsed)
{
	SimulatedIO simulatedInputSupplier(parameters.simulatedInput);
	simulatedInputHandle = &simulatedInputSupplier;
	std::list<Tag*> testResults = ioParseTestInstance->getTagsAsListParsedFrom(simulatedInputHandle);
	ASSERT_EQ(testResults.size(), parameters.expectedNumOfTags) << "\nTest Failed on: " << parameters.simulatedInput;
}


INSTANTIATE_TEST_CASE_P(default, XMLTagParserTest,
	testing::Values(
	//Parameterised Tests take form:
	//	{"input", expected num of tags parsed from that input}
	XMLTagCountParams{ "<s></s t=\"a\">", 2 },
	XMLTagCountParams{"<this is=\"atest\">", 1 },
	XMLTagCountParams{ "   ", 0 },
	XMLTagCountParams{ "", 0 },
	XMLTagCountParams{ ">a <b <>", 3 },
	XMLTagCountParams{ "<name s=\">\">", 1 },
	XMLTagCountParams{ "<t is = \"a test of< > will\"  >", 1 },
	XMLTagCountParams{ "a <b <>", 2 },
	XMLTagCountParams{ "<a <b <>", 3 },
	XMLTagCountParams{ "<a <b>", 2 },
	XMLTagCountParams{ "<a></a>    > <>   <>", 5 },
	XMLTagCountParams{ " one unopened> tag", 1 },
	XMLTagCountParams{ "<try and=\"Parse\">  this is a the text to remove  <mtry l=\";\">", 2 },
	XMLTagCountParams{ " just plain text, no xml", 0 },
	XMLTagCountParams{ " one <unclosed tag", 1 },
	XMLTagCountParams{ "one tag <enclosed> inside plain text", 1 },
	XMLTagCountParams{ "<normalTag>", 1 },
	XMLTagCountParams{ "< normalTagWithLeadingSpace>", 1 },
	XMLTagCountParams{ "<closing></closing>", 2 },
	XMLTagCountParams{ "<selfClosing/>", 1 },
	XMLTagCountParams{ "<selfClosingWithSpace />", 1 },
	XMLTagCountParams{ "< selfClosingWithLeadingSpace/>", 1 },
	XMLTagCountParams{ "< selfClosingWithmultipleSpace  />", 1 },
	XMLTagCountParams{ "<>", 1 },
	XMLTagCountParams{ "</>", 1 },
	XMLTagCountParams{ "< >", 1 },
	XMLTagCountParams{ "<  >", 1 },
	XMLTagCountParams{ "< />", 1 },
	XMLTagCountParams{ "< / >", 1 },
	XMLTagCountParams{ "</ >", 1 },
	XMLTagCountParams{ "< >", 1 },
	XMLTagCountParams{ "<tag with=\"one attribute\"", 1 },
	XMLTagCountParams{ "<this><test has=\"many\"></test><and this=\"is\" multiple=\"spaces\"></and></this>", 6 }
	)
);

//------------------------- TESTING THE CONTENTS OF THE RETURNED TAGS -----------

struct XMLSingleParseContentsParam
{
	std::string simulatedInput;
	std::string expectedName[100];
};

class SingleParseTagContents : public testing::Test, public testing::WithParamInterface<XMLSingleParseContentsParam>{
public:
	XMLTagParser* ioParseTestInstance; //class under test
	XMLSingleParseContentsParam parameters = GetParam();
	IO* simulatedInputHandle;
};

TEST_P(SingleParseTagContents, ContentsWhenOneTagParsed)
{
	SimulatedIO simulatedInputSupplier(parameters.simulatedInput);
	simulatedInputHandle = &simulatedInputSupplier;
	std::list<Tag*> testResults = ioParseTestInstance->getTagsAsListParsedFrom(simulatedInputHandle);
	
	int i = 0;
	std::string expected;
	std::string actual;
	for each (Tag* current in testResults)
	{
		expected = parameters.expectedName[i];
		actual = current->getTagName();
		if (actual != expected)
		{
			ASSERT_EQ(parameters.expectedName[i], current->getTagName()) << "\nTest Failed on: " << parameters.simulatedInput;
		}
		i++;
	}
	ASSERT_EQ(1, 1);
}


INSTANTIATE_TEST_CASE_P(default, SingleParseTagContents,
	testing::Values(
	//Parameterised Tests take form:
	//	{"input", {"tagname", "tagname", "tagname"}}
	XMLSingleParseContentsParam{ "<test></test>", {"test", "test"} },
	XMLSingleParseContentsParam{ ">a <b <>", {">", "b", ""} },
	XMLSingleParseContentsParam{ "<name s=\">\">", {"name"} },
	XMLSingleParseContentsParam{ "<t is = \"a test of< > will\"  >", {"t"} },
	XMLSingleParseContentsParam{ "a <b <>", {"b", ""} },
	XMLSingleParseContentsParam{ "<a <b <>", {"a", "b", ""} },
	XMLSingleParseContentsParam{ "<a <b>", {"a", "b"} },
	XMLSingleParseContentsParam{ "<a></a>    > <>   <>", {"a", "a", ">", "", ""} },
	XMLSingleParseContentsParam{ " one unopened> tag", { ">" } },
	XMLSingleParseContentsParam{ "<try and=\"Parse\">  this is a the text to remove  <mtry l=\";\">", {"try", "mtry"} },
	XMLSingleParseContentsParam{ " just plain text, no xml", {} },
	XMLSingleParseContentsParam{ " one <unclosed tag", {"unclosed"} },
	XMLSingleParseContentsParam{ "one tag <enclosed> inside plain text", {"enclosed"} },
	XMLSingleParseContentsParam{ "<normalTag>", {"normalTag"} },
	XMLSingleParseContentsParam{ "< normalTagWithLeadingSpace>", {""} },
	XMLSingleParseContentsParam{ "<closing></closing>", {"closing", "closing"} },
	XMLSingleParseContentsParam{ "<selfClosing/>", {"selfClosing"} },
	XMLSingleParseContentsParam{ "<selfClosingWithSpace />", {"selfClosingWithSpace"} },
	XMLSingleParseContentsParam{ "< selfClosingWithLeadingSpace/>", {""} },
	XMLSingleParseContentsParam{ "< selfClosingWithmultipleSpace  />", {""} },
	XMLSingleParseContentsParam{ "<>", {""} },
	XMLSingleParseContentsParam{ "</>", {""} },
	XMLSingleParseContentsParam{ "< >", {""} },
	XMLSingleParseContentsParam{ "<  >", { "" } },
	XMLSingleParseContentsParam{ "< />", { "" } },
	XMLSingleParseContentsParam{ "< / >", { "" } },
	XMLSingleParseContentsParam{ "</ >", { "" } },
	XMLSingleParseContentsParam{ "< >", { "" } },
	XMLSingleParseContentsParam{ "<tag with=\"one attribute\"", { "tag" } },
	XMLSingleParseContentsParam{ "<this><test has=\"many\"></test><and this=\"is\" multiple=\"spaces\"></and></this>", {"this", "test", "test", "and", "and", "this"} }	
	)
);

//------------------------- TESTING THE ATTRIBUTES WHICH ARE IN THE TAGS POST PARSING -----------

struct XMLAttributeContentsParam
{
	std::string simulatedInput;
	std::string expectedAttrContents[100];
};

class ContentsOfAttributes : public testing::Test, public testing::WithParamInterface<XMLAttributeContentsParam>{
public:
	XMLTagParser* ioParseTestInstance; //class under test
	XMLAttributeContentsParam parameters = GetParam();
	IO* simulatedInputHandle;
};

TEST_P(ContentsOfAttributes, ContentsOfAttributes)
{
	SimulatedIO simulatedInputSupplier(parameters.simulatedInput);
	simulatedInputHandle = &simulatedInputSupplier;
	std::list<Tag*> testResults = ioParseTestInstance->getTagsAsListParsedFrom(simulatedInputHandle);

	int i = 0;
	std::list<std::string> attrResults;
	std::string expected;
	std::string actual;
	for each (Tag* currentTag in testResults)
	{
		attrResults = currentTag->getAttributes();
		if (attrResults.empty())
		{
			i++;
		}
		else
		{
			for each (std::string currentAttribute in attrResults)
			{
				expected = parameters.expectedAttrContents[i];
				actual = currentAttribute;
				if (expected != actual)
				{
					ASSERT_EQ(expected, actual) << "\nTest Failed on: " << parameters.simulatedInput << " at attribute " << i << ": " << expected;
				}
				i++;
			}
		}
	}
	ASSERT_EQ(1, 1);
}


INSTANTIATE_TEST_CASE_P(default, ContentsOfAttributes,
	testing::Values(
	//Parameterised Tests take form:
	//	{"input", {"tagname", "tagname", "tagname"}}
	XMLAttributeContentsParam{ "<a test= test2= >", { "test=", "test=" } },
	XMLAttributeContentsParam{ "<simple test=\"is\"></simple>", {"test=\"is\""} },
	XMLAttributeContentsParam{ "<s></s t=\"a\">", { "", "t=\"a\"" } },
	XMLAttributeContentsParam{ "<s   t=\"a\"", { "t=\"a\"" } },
	XMLAttributeContentsParam{ "<s t=\"a\" s=\"a\"", { "t=\"a\"", "s=\"a\"" } },
	XMLAttributeContentsParam{ "<s t=\"a\" t=\"a\" tsdf=\"as  df\">", { "t=\"a\"", "t=\"a\"", "tsdf=\"as  df\"" } },
	XMLAttributeContentsParam{ "tsdf=\"as  df\"", { "" } }, //no attribute here as all occurs outside tag
	XMLAttributeContentsParam{ "<a test=>", { "test=" } },
	XMLAttributeContentsParam{ "<a test>", { "test" } },
	XMLAttributeContentsParam{ "<a =\"test\" >", { "=\"test\"" } },
	XMLAttributeContentsParam{ "<a \"test\">", { "\"test\"" } },
	XMLAttributeContentsParam{ "", { "" } },
	XMLAttributeContentsParam{ "", { "" } }
	)
	);
	


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
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

struct XMLTagNameTestParams
{
	std::string simulatedInput;
	std::string expectedName;
};

class XMLTagParserTest : public testing::Test, public testing::WithParamInterface<XMLTagNameTestParams>{
public:
	ParserFactory parserSupplier;
	TagParserInterface* classUnderTest = parserSupplier.createParserFor("xml"); //class under test
	XMLTagNameTestParams parameters = GetParam();
	IO* simulatedInputHandle;
};

TEST_P(XMLTagParserTest, TestContentsOfNameParse)
{

	SimulatedIO input(parameters.simulatedInput);
	simulatedInputHandle = &input;
	std::vector<Tag*> resultsOfTest = classUnderTest->getTagsAsVectorParsedFrom(simulatedInputHandle->getContent());
	std::string parsedResult = resultsOfTest.front()->getTagName();
	ASSERT_EQ(parameters.expectedName, parsedResult);
}

INSTANTIATE_TEST_CASE_P(default, XMLTagParserTest,
	testing::Values(
	XMLTagNameTestParams{ "<>", "" },
	XMLTagNameTestParams{ "<t >", "t" },
	XMLTagNameTestParams{ "<test with=\"attributes\">", "test" },
	XMLTagNameTestParams{ "<test    >", "test" },
	XMLTagNameTestParams{ "< test>", "" }, //name is everything before first space in tag, in this case test is an attribute name
	XMLTagNameTestParams{ "<test >", "test" },
	XMLTagNameTestParams{"<test>", "test"}
));
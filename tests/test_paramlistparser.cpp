#include "assert_exception.h"
#include <gtest/gtest.h>
#include <figcone_shoal/parser.h>
#include <stream.h>
#include <paramparser.h>
#include <sstream>

namespace test_paramlistparser {

auto parseParam(const std::string& str)
{
    auto input = std::stringstream{str};
    auto stream = figcone::shoal::detail::Stream{input};
    return figcone::shoal::detail::parseParam(stream);
}

TEST(TestParamListParser, Basic)
{
    auto [paramName, param] = parseParam("testIntList = 1, 2, 3");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"1", "2", "3"}));
}

TEST(TestParamListParser, BasicWithoutMacro)
{
    auto [paramName, param] = parseParam("testIntList = 1, 2, 3");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"1", "2", "3"}));
}

TEST(TestParamListParser, BasicAltWhitespace)
{
    auto [paramName, param] = parseParam("testIntList =1,2,3 ");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"1", "2", "3"}));
}

TEST(TestParamListParser, EmptyLastElementError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =1,2, ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:19] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, EmptyFirstElementError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList = ,1,2 ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:15] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, EmptyElementError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList = 1, ,3 ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:18] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, EmptyLastElementAltWhitespaceError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =1,2, \n ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:19] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, EmptyError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList = ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:15] Parameter 'testIntList' value is missing");
    });
}

TEST(TestParamListParser, EmptyAltWhitespaceError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:14] Parameter 'testIntList' value is missing");
    });
}

TEST(TestParamListParser, EmptyAltWhitespace2Error)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList = \n  ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:15] Wrong param 'testIntList' format: parameter's value must be placed on the same line as its name");
    });
}

TEST(TestParamListParser, EmptyWithSeparatorError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList = ,");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:15] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, EmptyWithSeparatorAltWhitespaceError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =, ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:14] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, BasicString)
{
    auto [paramName, param] = parseParam("testStrList = 'Hello,\n world', Foo ");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testStrList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"Hello,\n world", "Foo"}));
}

TEST(TestParamListParser, Multiline)
{
    auto [paramName, param] = parseParam("testIntList = [1,2,3]");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"1", "2", "3"}));
}

TEST(TestParamListParser, MultilineWithString)
{
    auto [paramName, param] = parseParam("testStrList = [\"Hello\",world, ' ']");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testStrList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"Hello", "world", " "}));
}

TEST(TestParamListParser, MultilineWithStringAltWhitespace)
{
    auto [paramName, param] = parseParam("testStrList = [ \"Hello\"\n,  world,\n ''\n]");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testStrList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"Hello", "world", ""}));
}

TEST(TestParamListParser, MultilineAltWhitespace)
{
    auto [paramName, param] = parseParam("testIntList = [ 1, 2, 3 ]");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"1", "2", "3"}));
}

TEST(TestParamListParser, MultilineAltWhitespace2)
{
    auto [paramName, param] = parseParam("testIntList = [1,\n 2\n, 3 ]");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"1", "2", "3"}));
}

TEST(TestParamListParser, MultilineAltWhitespace3)
{
    auto [paramName, param] = parseParam("testIntList = [\n1\n,2,\n 3]");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"1", "2", "3"}));
}

TEST(TestParamListParser, MultilineTest)
{
    auto [paramName, param] = parseParam("testStrList = [mp4(h264), webm(vp8;vp9)]");
    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testStrList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{"mp4(h264)", "webm(vp8;vp9)"}));
}

TEST(TestParamListParser, MultiLineEmptyFirstElementError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =[,2,3 ] ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:15] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, MultiLineEmptyElementError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =[1, ,3 ] ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:18] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, MultiLineEmptyLastElementError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =[1,2, ] ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:20] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, MultiLineEmpty)
{
    auto [paramName, param] = parseParam("testIntList = [] ");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{}));
}

TEST(TestParamListParser, MultiLineEmptyAltWhitespace)
{
    auto [paramName, param] = parseParam("testIntList = [  ] ");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{}));
}

TEST(TestParamListParser, MultiLineEmptyAltWhitespace2)
{
    auto [paramName, param] = parseParam("testIntList = [ \n  ] ");

    ASSERT_TRUE(param.isList());
    EXPECT_EQ(paramName, "testIntList");
    EXPECT_EQ(param.valueList(), (std::vector<std::string>{}));
}

TEST(TestParamListParser, MultiLineEmptyWithSeparatorError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =[,] ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:15] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, MultiLineEmptyWithSeparatorAltWhitespaceError)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =[  , ] ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:1, column:17] Parameter list 'testIntList' element is missing");
    });
}

TEST(TestParamListParser, MultiLineEmptyWithSeparatorAltWhitespace2Error)
{
    assert_exception<figcone::ConfigError>([&]{
        parseParam("testIntList =[  \n, \n] ");
    }, [](const figcone::ConfigError& e){
        ASSERT_EQ(std::string{e.what()}, "[line:2, column:1] Parameter list 'testIntList' element is missing");
    });
}

}

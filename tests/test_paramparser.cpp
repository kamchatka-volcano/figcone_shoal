#include "assert_exception.h"
#include <paramparser.h>
#include <stream.h>
#include <figcone_shoal/parser.h>
#include <gtest/gtest.h>
#include <sstream>

namespace test_paramparser {

auto parseParam(const std::string& str)
{
    auto input = std::stringstream{str};
    auto stream = figcone::shoal::detail::Stream{input};
    return figcone::shoal::detail::parseParam(stream);
}

TEST(TestParamParser, ParamWhitespaces)
{
    auto [paramName, param] = parseParam(" \ntest = 1  \n");
    ASSERT_TRUE(param.isItem());
    EXPECT_EQ(paramName, "test");
    EXPECT_EQ(param.value(), "1");
}

TEST(TestParamParser, IntParam)
{
    auto [paramName, param] = parseParam("test=1");
    ASSERT_TRUE(param.isItem());
    EXPECT_EQ(paramName, "test");
    EXPECT_EQ(param.value(), "1");
}

TEST(TestParamParser, StringParam)
{
    auto [paramName, param] = parseParam("test='hello world.txt'");
    ASSERT_TRUE(param.isItem());
    EXPECT_EQ(paramName, "test");
    EXPECT_EQ(param.value(), "hello world.txt");
}

TEST(TestParamParser, EmptyStringParam)
{
    auto [paramName, param] = parseParam("test=''");
    ASSERT_TRUE(param.isItem());
    EXPECT_EQ(paramName, "test");
    EXPECT_EQ(param.value(), "");
}

TEST(TestParamParser, Multiword)
{
    auto [paramName, param] = parseParam("test='hello world'");
    ASSERT_TRUE(param.isItem());
    EXPECT_EQ(paramName, "test");
    EXPECT_EQ(param.value(), "hello world");
}

TEST(TestParamParser, Multiword2)
{
    auto [paramName, param] = parseParam("test='hello\n world'");
    ASSERT_TRUE(param.isItem());
    EXPECT_EQ(paramName, "test");
    EXPECT_EQ(param.value(), "hello\n world");
}

TEST(TestParamParser, Multiword3)
{
    auto [paramName, param] = parseParam("test=\"hello\n world\"");
    ASSERT_TRUE(param.isItem());
    EXPECT_EQ(paramName, "test");
    EXPECT_EQ(param.value(), "hello\n world");
}

TEST(TestParamParser, ParamWithoutAssignmentError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parseParam("test");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:1, column:5] Wrong param 'test' format: missing '='");
            });
}

TEST(TestParamParser, ParamWithoutValueError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parseParam("test =");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:1, column:7] Parameter 'test' value is missing");
            });
}

TEST(TestParamParser, MultiwordParamNameError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parseParam("test error= 1");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:1, column:6] Wrong param 'test' format: missing '='");
            });
}

TEST(TestParamParser, MultilineParamNameError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parseParam("test \n=1");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(
                        std::string{error.what()},
                        "[line:1, column:6] Wrong param 'test' format: parameter's value must be placed on the same "
                        "line as its name");
            });
}

TEST(TestParamParser, MultilineParamError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parseParam("test = \n1");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(
                        std::string{error.what()},
                        "[line:1, column:8] Wrong param 'test' format: "
                        "parameter's value must be placed on the same line as its name");
            });
}

TEST(TestParamParser, UnclosedStringParamError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parseParam("test='Hello ");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:1, column:6] String isn't closed");
            });
}

} //namespace test_paramparser

#include "assert_exception.h"
#include <figcone_shoal/parser.h>
#include <gtest/gtest.h>

#include <sstream>

namespace test_nodeparser {

auto parse(const std::string& str)
{
    auto input = std::stringstream{str};
    auto parser = figcone::shoal::Parser{};
    return parser.parse(input);
}

TEST(TestNodeParser, SingleNodeSingleLevel)
{
    auto result = parse(R"(
        foo = 5
        bar = test
        #a:
          testInt = 10
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 2);
    ASSERT_EQ(tree.hasParam("foo"), 1);
    ASSERT_EQ(tree.hasParam("bar"), 1);
    EXPECT_EQ(tree.param("foo").value(), "5");
    EXPECT_EQ(tree.param("bar").value(), "test");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_EQ(tree.hasNode("a"), 1);
    auto& aNode = tree.node("a").asItem();
    ASSERT_EQ(aNode.paramsCount(), 1);
    EXPECT_EQ(aNode.param("testInt").value(), "10");
}

TEST(TestNodeParser, MultiNodeSingleLevel)
{

    auto result = parse(R"(
        foo = 5
        bar = test
        #a:
          testInt = 10
        -
        #b:
          testInt = 11
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 2);
    ASSERT_EQ(tree.hasParam("foo"), 1);
    ASSERT_EQ(tree.hasParam("bar"), 1);
    EXPECT_EQ(tree.param("foo").value(), "5");
    EXPECT_EQ(tree.param("bar").value(), "test");
    ASSERT_EQ(tree.nodesCount(), 2);
    ASSERT_EQ(tree.hasNode("a"), 1);
    ASSERT_EQ(tree.hasNode("b"), 1);
    auto& aNode = tree.node("a").asItem();
    ASSERT_EQ(aNode.paramsCount(), 1);
    EXPECT_EQ(aNode.param("testInt").value(), "10");
    auto& bNode = tree.node("b").asItem();
    ASSERT_EQ(bNode.paramsCount(), 1);
    EXPECT_EQ(bNode.param("testInt").value(), "11");
}

TEST(TestNodeParser, MultiLevel)
{
    auto result = parse(R"(
        foo = 5
        bar = test
        #c:
          testInt = 11
          testDouble = 12
          #b:
            testInt = 10
            testString = 'Hello world'
        ---
        #b:
          testInt = 9
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 2);
    ASSERT_EQ(tree.hasParam("foo"), 1);
    ASSERT_EQ(tree.hasParam("bar"), 1);
    EXPECT_EQ(tree.param("foo").value(), "5");
    EXPECT_EQ(tree.param("bar").value(), "test");
    ASSERT_EQ(tree.nodesCount(), 2);
    ASSERT_EQ(tree.hasNode("c"), 1);
    ASSERT_EQ(tree.hasNode("b"), 1);

    auto& cNode = tree.node("c").asItem();
    ASSERT_EQ(cNode.paramsCount(), 2);
    EXPECT_EQ(cNode.param("testInt").value(), "11");
    EXPECT_EQ(cNode.param("testDouble").value(), "12");
    ASSERT_EQ(cNode.nodesCount(), 1);
    ASSERT_EQ(cNode.hasNode("b"), 1);
    auto& cbNode = cNode.node("b").asItem();
    ASSERT_EQ(cbNode.paramsCount(), 2);
    EXPECT_EQ(cbNode.param("testInt").value(), "10");
    EXPECT_EQ(cbNode.param("testString").value(), "Hello world");

    auto& bNode = tree.node("b").asItem();
    ASSERT_EQ(bNode.paramsCount(), 1);
    EXPECT_EQ(bNode.param("testInt").value(), "9");
}

TEST(TestNodeParser, MultiLevelWithComments)
{
    auto result = parse(R"(
        foo = 5;
        bar = test;
        #c:                             ;test comment#1
          testInt = 11                  ;test comment#2
          testDouble = 12
        ;test comment#3
          #b:                           ;test comment#4
            testInt = 10
            testString = 'Hello; world'
          --b; test comment#5
        ---;test comment#6
        #b:
          testInt = 9
        -                               ;test comment#7
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 2);
    ASSERT_EQ(tree.hasParam("foo"), 1);
    ASSERT_EQ(tree.hasParam("bar"), 1);
    EXPECT_EQ(tree.param("foo").value(), "5");
    EXPECT_EQ(tree.param("bar").value(), "test");
    ASSERT_EQ(tree.nodesCount(), 2);
    ASSERT_EQ(tree.hasNode("c"), 1);
    ASSERT_EQ(tree.hasNode("b"), 1);

    auto& cNode = tree.node("c").asItem();
    ASSERT_EQ(cNode.paramsCount(), 2);
    EXPECT_EQ(cNode.param("testInt").value(), "11");
    EXPECT_EQ(cNode.param("testDouble").value(), "12");
    ASSERT_EQ(cNode.nodesCount(), 1);
    ASSERT_EQ(cNode.hasNode("b"), 1);
    auto& cbNode = cNode.node("b").asItem();
    ASSERT_EQ(cbNode.paramsCount(), 2);
    EXPECT_EQ(cbNode.param("testInt").value(), "10");
    EXPECT_EQ(cbNode.param("testString").value(), "Hello; world");

    auto& bNode = tree.node("b").asItem();
    ASSERT_EQ(bNode.paramsCount(), 1);
    EXPECT_EQ(bNode.param("testInt").value(), "9");
}

TEST(TestNodeParser, MultiLevelParamAfterNode)
{
    auto result = parse(R"(
        foo = 5
        bar = test
        #c:
          testInt = 11
          #b:
            testInt = 10
            testString = 'Hello world'
          -
          testDouble = 12
        ---
        #b:
          testInt = 9
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 2);
    ASSERT_EQ(tree.hasParam("foo"), 1);
    ASSERT_EQ(tree.hasParam("bar"), 1);
    EXPECT_EQ(tree.param("foo").value(), "5");
    EXPECT_EQ(tree.param("bar").value(), "test");
    ASSERT_EQ(tree.nodesCount(), 2);
    ASSERT_EQ(tree.hasNode("c"), 1);
    ASSERT_EQ(tree.hasNode("b"), 1);
    auto& cNode = tree.node("c").asItem();
    ASSERT_EQ(cNode.paramsCount(), 2);
    ASSERT_EQ(cNode.hasParam("testInt"), 1);
    EXPECT_EQ(cNode.param("testInt").value(), "11");
    ASSERT_EQ(cNode.hasParam("testDouble"), 1);
    EXPECT_EQ(cNode.param("testDouble").value(), "12");
    ASSERT_EQ(cNode.nodesCount(), 1);
    ASSERT_EQ(cNode.hasNode("b"), 1);
    auto& cbNode = cNode.node("b").asItem();
    ASSERT_EQ(cbNode.paramsCount(), 2);
    EXPECT_EQ(cbNode.param("testInt").value(), "10");
    EXPECT_EQ(cbNode.param("testString").value(), "Hello world");
    auto& bNode = tree.node("b").asItem();
    ASSERT_EQ(bNode.paramsCount(), 1);
    EXPECT_EQ(bNode.param("testInt").value(), "9");
}

TEST(TestNodeParser, MultiLevelCloseByName)
{
    auto result = parse(R"(
        foo = 5
        bar = test
        #c:
          testInt = 11
          testDouble = 12
          #b:
            testInt = 10
            testString = 'Hello world'
        --c
        #b:
          testInt = 9
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 2);
    ASSERT_EQ(tree.hasParam("foo"), 1);
    ASSERT_EQ(tree.hasParam("bar"), 1);
    EXPECT_EQ(tree.param("foo").value(), "5");
    EXPECT_EQ(tree.param("bar").value(), "test");
    ASSERT_EQ(tree.nodesCount(), 2);
    ASSERT_EQ(tree.hasNode("c"), 1);
    ASSERT_EQ(tree.hasNode("b"), 1);
    auto& cNode = tree.node("c").asItem();
    ASSERT_EQ(cNode.paramsCount(), 2);
    ASSERT_EQ(cNode.hasParam("testInt"), 1);
    EXPECT_EQ(cNode.param("testInt").value(), "11");
    ASSERT_EQ(cNode.hasParam("testDouble"), 1);
    EXPECT_EQ(cNode.param("testDouble").value(), "12");
    ASSERT_EQ(cNode.nodesCount(), 1);
    ASSERT_EQ(cNode.hasNode("b"), 1);
    auto& cbNode = cNode.node("b").asItem();
    ASSERT_EQ(cbNode.paramsCount(), 2);
    EXPECT_EQ(cbNode.param("testInt").value(), "10");
    EXPECT_EQ(cbNode.param("testString").value(), "Hello world");
    auto& bNode = tree.node("b").asItem();
    ASSERT_EQ(bNode.paramsCount(), 1);
    EXPECT_EQ(bNode.param("testInt").value(), "9");
}

TEST(TestNodeParser, MultiLevelClose)
{
    auto result = parse(R"(
        foo = 5
        bar = test
        #c:
          testInt = 11
          testDouble = 12
          #b:
            testInt = 10
            testString = 'Hello world'
          -
        -
        #b:
          testInt = 9
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 2);
    ASSERT_EQ(tree.hasParam("foo"), 1);
    ASSERT_EQ(tree.hasParam("bar"), 1);
    EXPECT_EQ(tree.param("foo").value(), "5");
    EXPECT_EQ(tree.param("bar").value(), "test");
    ASSERT_EQ(tree.nodesCount(), 2);
    ASSERT_EQ(tree.hasNode("c"), 1);
    ASSERT_EQ(tree.hasNode("b"), 1);
    auto& cNode = tree.node("c").asItem();
    ASSERT_EQ(cNode.paramsCount(), 2);
    ASSERT_EQ(cNode.hasParam("testInt"), 1);
    EXPECT_EQ(cNode.param("testInt").value(), "11");
    ASSERT_EQ(cNode.hasParam("testDouble"), 1);
    EXPECT_EQ(cNode.param("testDouble").value(), "12");
    ASSERT_EQ(cNode.nodesCount(), 1);
    ASSERT_EQ(cNode.hasNode("b"), 1);
    auto& cbNode = cNode.node("b").asItem();
    ASSERT_EQ(cbNode.paramsCount(), 2);
    EXPECT_EQ(cbNode.param("testInt").value(), "10");
    EXPECT_EQ(cbNode.param("testString").value(), "Hello world");
    auto& bNode = tree.node("b").asItem();
    ASSERT_EQ(bNode.paramsCount(), 1);
    EXPECT_EQ(bNode.param("testInt").value(), "9");
}

TEST(TestNodeParser, CloseByUnknownNameError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parse(R"(
            foo = 5
            bar = test
            #c:
              testInt = 11
              testDouble = 12
              #b:
                testInt = 10
                testString = 'Hello world'
            --test
            #b:
              testInt = 9
        )");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:10, column:14] Can't close unexisting node 'test'");
            });
}

TEST(TestNodeParser, ReturnToRootClosingTokenError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parse(R"(
            foo = 5
            bar = test
            #a:
              testInt = 10
            ---a
        )");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:6, column:16] Invalid closing token '---a'");
            });
}

TEST(TestNodeParser, ReturnToParentClosingTokenError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parse(R"(
            foo = 5
            bar = test
            #a:
              testInt = 10
            -a
        )");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:6, column:14] Invalid closing token '-a'");
            });
}

TEST(TestNodeParser, InvalidNodeNameLineError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parse(R"(
            foo = 5
            bar = test
            #a:  test
              testInt = 10
        )");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(
                        std::string{error.what()},
                        "[line:4, column:16] Wrong config node 'a' format: "
                        "only whitespaces and comments can be placed on the same line with config node's name.");
            });
}

TEST(TestNodeParser, BlankNodeNameError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parse(R"(
            foo = 5
            bar = test
            # :
              testInt = 10
        )");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:4, column:13] Config node name can't be blank");
            });
}

TEST(TestNodeParser, MultilineNodeNameError)
{
    assert_exception<figcone::ConfigError>(
            [&]
            {
                parse(R"(
            foo = 5
            bar = test
            #
            b:
              testInt = 10
        )");
            },
            [](const figcone::ConfigError& error)
            {
                EXPECT_EQ(std::string{error.what()}, "[line:4, column:14] Config node can't have a multiline name");
            });
}

} //namespace test_nodeparser
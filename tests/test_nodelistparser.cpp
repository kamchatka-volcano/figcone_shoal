#include "assert_exception.h"
#include <figcone_shoal/parser.h>
#include <gtest/gtest.h>

namespace test_nodelistparser {

auto parse(const std::string& str)
{
    auto input = std::stringstream{str};
    auto parser = figcone::shoal::Parser{};
    return parser.parse(input);
}

TEST(TestNodeListParser, Basic)
{
    auto result = parse(R"(
        testStr = Hello
        #testNodes:
        ###
            testInt = 3
        ###
            testInt = 2
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_EQ(tree.hasParam("testStr"), 1);
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_EQ(tree.hasNode("testNodes"), 1);
    auto& testNodes = tree.node("testNodes").asList();
    ASSERT_EQ(testNodes.count(), 2);
    {
        auto& nodeData = testNodes.node(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.node(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}

TEST(TestNodeListParser, BasicClosedManually)
{
    auto result = parse(R"(
        #testNodes:
        ###
            testInt = 3
        ###
            testInt = 2
        -
        testStr = Hello
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_EQ(tree.hasParam("testStr"), 1);
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_EQ(tree.hasNode("testNodes"), 1);
    auto& testNodes = tree.node("testNodes").asList();
    ASSERT_EQ(testNodes.count(), 2);
    {
        auto& nodeData = testNodes.node(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.node(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}

TEST(TestNodeListParser, BasicClosedByName)
{

    auto result = parse(R"(
        #testNodes:
        ###
            testInt = 3
        ###
            testInt = 2
        --testNodes
        testStr = Hello
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_EQ(tree.hasParam("testStr"), 1);
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_EQ(tree.hasNode("testNodes"), 1);
    auto& testNodes = tree.node("testNodes").asList();
    ASSERT_EQ(testNodes.count(), 2);
    {
        auto& nodeData = testNodes.node(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.node(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}

TEST(TestNodeListParser, NestedCloseToRoot)
{
    auto result = parse(R"(
        #testCfg:
            testStr = Hello
            #testNodes:
            ###
                testInt = 3
            ###
                testInt = 2
        ---
        testDouble = 0.5
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_EQ(tree.hasParam("testDouble"), 1);
    EXPECT_EQ(tree.param("testDouble").value(), "0.5");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_EQ(tree.hasNode("testCfg"), 1);
    auto& testCfg = tree.node("testCfg").asItem();
    ASSERT_EQ(testCfg.paramsCount(), 1);
    ASSERT_EQ(testCfg.hasParam("testStr"), 1);
    EXPECT_EQ(testCfg.param("testStr").value(), "Hello");
    ASSERT_EQ(testCfg.nodesCount(), 1);
    ASSERT_EQ(testCfg.hasNode("testNodes"), 1);
    auto& testNodes = testCfg.node("testNodes").asList();
    ASSERT_EQ(testNodes.count(), 2);
    {
        auto& nodeData = testNodes.node(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.node(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}

TEST(TestNodeListParser, NestedClosedByName)
{
    auto result = parse(R"(
        #testCfg:
            testStr = Hello
            #testNodes:
            ###
                testInt = 3
            ###
                testInt = 2
        --testCfg
        testDouble = 0.5
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_EQ(tree.hasParam("testDouble"), 1);
    EXPECT_EQ(tree.param("testDouble").value(), "0.5");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_EQ(tree.hasNode("testCfg"), 1);
    auto& testCfg = tree.node("testCfg").asItem();
    ASSERT_EQ(testCfg.paramsCount(), 1);
    ASSERT_EQ(testCfg.hasParam("testStr"), 1);
    EXPECT_EQ(testCfg.param("testStr").value(), "Hello");
    ASSERT_EQ(testCfg.nodesCount(), 1);
    ASSERT_EQ(testCfg.hasNode("testNodes"), 1);
    auto& testNodes = testCfg.node("testNodes").asList();
    ASSERT_EQ(testNodes.count(), 2);
    {
        auto& nodeData = testNodes.node(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testNodes.node(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testInt"), 1);
        EXPECT_EQ(nodeData.param("testInt").value(), "2");
    }
}

TEST(TestNodeListParser, NestedCfgList)
{
   auto result = parse(R"(
        #testList:
        ###
            testStr = Hello
            #testNodes:
            ###
                testInt = 3
            ###
                testInt = 33
            -
        ###
            #testNodes:
            ###
                testInt = 5
            -
            testStr = World
        -
        testStr = Hello
    )");

   auto& tree = result.asItem();
   ASSERT_EQ(tree.paramsCount(), 1);
   ASSERT_EQ(tree.hasParam("testStr"), 1);
   EXPECT_EQ(tree.param("testStr").value(), "Hello");
   ASSERT_EQ(tree.nodesCount(), 1);
   ASSERT_EQ(tree.hasNode("testList"), 1);
   auto& testList = tree.node("testList").asList();
   ASSERT_EQ(testList.count(), 2);
   {
       auto& nodeData = testList.node(0).asItem();
       ASSERT_EQ(nodeData.paramsCount(), 1);
       ASSERT_EQ(nodeData.hasParam("testStr"), 1);
       EXPECT_EQ(nodeData.param("testStr").value(), "Hello");
       ASSERT_EQ(nodeData.nodesCount(), 1);
       ASSERT_EQ(nodeData.hasNode("testNodes"), 1);
       auto& testNodes = nodeData.node("testNodes").asList();
       ASSERT_EQ(testNodes.count(), 2);
       {
           auto& nodeData = testNodes.node(0).asItem();
           ASSERT_EQ(nodeData.paramsCount(), 1);
           ASSERT_EQ(nodeData.hasParam("testInt"), 1);
           EXPECT_EQ(nodeData.param("testInt").value(), "3");
       }
       {
           auto& nodeData = testNodes.node(1).asItem();
           ASSERT_EQ(nodeData.paramsCount(), 1);
           ASSERT_EQ(nodeData.hasParam("testInt"), 1);
           EXPECT_EQ(nodeData.param("testInt").value(), "33");
       }
   }
   {
       auto& nodeData = testList.node(1).asItem();
       ASSERT_EQ(nodeData.paramsCount(), 1);
       ASSERT_EQ(nodeData.hasParam("testStr"), 1);
       EXPECT_EQ(nodeData.param("testStr").value(), "World");
       ASSERT_EQ(nodeData.nodesCount(), 1);
       ASSERT_EQ(nodeData.hasNode("testNodes"), 1);
       auto& testNodes = nodeData.node("testNodes").asList();
       ASSERT_EQ(testNodes.count(), 1);
       {
           auto& nodeData = testNodes.node(0).asItem();
           ASSERT_EQ(nodeData.paramsCount(), 1);
           ASSERT_EQ(nodeData.hasParam("testInt"), 1);
           EXPECT_EQ(nodeData.param("testInt").value(), "5");
       }
   }
}

TEST(TestNodeListParser, NestedCfg2List)
{
    auto result = parse(R"(
        #testList:
        ###
            testDouble = 3.0
            #testNode:
                testInt = 3
        ###
            testDouble = 5.0
            #testNode:
                testInt = 5
        --testList
        testStr = Hello
    )");

    auto& tree = result.asItem();
    ASSERT_EQ(tree.paramsCount(), 1);
    ASSERT_EQ(tree.hasParam("testStr"), 1);
    EXPECT_EQ(tree.param("testStr").value(), "Hello");
    ASSERT_EQ(tree.nodesCount(), 1);
    ASSERT_EQ(tree.hasNode("testList"), 1);
    auto& testList = tree.node("testList").asList();
    ASSERT_EQ(testList.count(), 2);
    {
        auto& nodeData = testList.node(0).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testDouble"), 1);
        EXPECT_EQ(nodeData.param("testDouble").value(), "3.0");
        ASSERT_EQ(nodeData.nodesCount(), 1);
        ASSERT_EQ(nodeData.hasNode("testNode"), 1);
        auto& testNode = nodeData.node("testNode").asItem();
        ASSERT_EQ(testNode.paramsCount(), 1);
        ASSERT_EQ(testNode.hasParam("testInt"), 1);
        EXPECT_EQ(testNode.param("testInt").value(), "3");
    }
    {
        auto& nodeData = testList.node(1).asItem();
        ASSERT_EQ(nodeData.paramsCount(), 1);
        ASSERT_EQ(nodeData.hasParam("testDouble"), 1);
        EXPECT_EQ(nodeData.param("testDouble").value(), "5.0");
        ASSERT_EQ(nodeData.nodesCount(), 1);
        ASSERT_EQ(nodeData.hasNode("testNode"), 1);
        auto& testNode = nodeData.node("testNode").asItem();
        ASSERT_EQ(testNode.paramsCount(), 1);
        ASSERT_EQ(testNode.hasParam("testInt"), 1);
        EXPECT_EQ(testNode.param("testInt").value(), "5");
    }
}


TEST(TestNodeListParser, InvalidListSeparatorLineError)
{
    assert_exception<figcone::ConfigError>([&]{
    parse(R"(
        #testNodes:
        ### error
            testInt = 3
        ###
            testInt = 2
        -
        testStr = Hello
    )");}, [](const figcone::ConfigError& error){
        EXPECT_EQ(std::string{error.what()}, "[line:3, column:13] Wrong config node list 'testNodes' format: "
                                             "there can't be anything besides comments and whitespaces "
                                             "on the same line with list separator '###'");
    });
}

TEST(TestNodeListParser, InvalidListSeparatorError)
{
    assert_exception<figcone::ConfigError>([&]{
        parse(R"(
        #testNodes:
        ##error
            testInt = 3
        ###
            testInt = 2
        -
        testStr = Hello
    )");}, [](const figcone::ConfigError& error){
        EXPECT_EQ(std::string{error.what()}, "[line:3, column:16] Config node can't have a multiline name");
    });
}

TEST(TestNodeListParser, InvalidListError)
{
    assert_exception<figcone::ConfigError>([&]{
        parse(R"(
        #testNodes:
        error
        ###
            testInt = 3
        ###
            testInt = 2
        -
        testStr = Hello
    )");}, [](const figcone::ConfigError& error){
        EXPECT_EQ(std::string{error.what()}, "[line:3, column:14] Wrong param 'error' format: parameter's value must be placed on the same line as its name");
    });
}

}

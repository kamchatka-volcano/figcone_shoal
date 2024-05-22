#include "nodeparser.h"
#include "paramparser.h"
#include "stream.h"
#include "utils.h"
#include <figcone_tree/errors.h>
#include <figcone_tree/tree.h>
#include <gsl/assert>

namespace figcone::shoal::detail {

std::string readNodeName(Stream& stream)
{
    const auto firstChar = stream.read();
    Expects(firstChar == "#");

    auto nodeName = std::string{};
    while (!stream.atEnd()) {
        if (stream.peek() == "\n")
            throw ConfigError{"Config node can't have a multiline name", stream.position()};

        if (stream.peek() == ":") {
            stream.skip(1);
            const auto pos = stream.position();
            if (!isBlank(readUntil(stream, "\n")))
                throw ConfigError{
                        "Wrong config node '" + nodeName +
                                "' format: only whitespaces and comments can be placed "
                                "on the same line with config node's name.",
                        pos};
            break;
        }
        nodeName += stream.read();
    }
    return nodeName;
}

ConfigReadResult readEndToken(Stream& stream)
{
    stream.skip(1);
    if (stream.atEnd() || std::isspace(stream.peek().front()))
        return {ConfigReadResult::NextAction::ReturnToParentNode, {}, {}};

    if (stream.peek(2) == "--") {
        stream.skip(2);
        if (!stream.peek().empty() && !std::isspace(stream.peek().front()))
            throw ConfigError{"Invalid closing token '---" + stream.peek() + "'", stream.position()};

        return {ConfigReadResult::NextAction::ReturnToRootNode, {}, {}};
    }

    const auto pos = stream.position();
    const auto nextChar = stream.read();
    if (nextChar != "-")
        throw ConfigError{"Invalid closing token '-" + nextChar + "'", pos};

    const auto parentConfigNode = readWord(stream);
    return {ConfigReadResult::NextAction::ReturnToNodeByName, parentConfigNode, pos};
}

ConfigReadResult checkReadResult(
        const ConfigReadResult& readResult,
        const std::string& newNodeName,
        const figcone::TreeNode& parentNode)
{
    if (readResult.nextAction == ConfigReadResult::NextAction::ReturnToRootNode)
        return parentNode.isRoot() ? ConfigReadResult{ConfigReadResult::NextAction::ContinueReading, {}, {}}
                                   : readResult;

    if (readResult.nextAction == ConfigReadResult::NextAction::ReturnToParentNode && parentNode.isList()) {
        if (parentNode.isRoot())
            throw ConfigError{"Can't close root node", readResult.returnToNodeStreamPosition};
        else
            return readResult;
    }

    if (readResult.nextAction == ConfigReadResult::NextAction::ReturnToNodeByName) {
        if (newNodeName != readResult.parentNodeName) {
            if (parentNode.isRoot())
                throw ConfigError{
                        "Can't close unexisting node '" + readResult.parentNodeName + "'",
                        readResult.returnToNodeStreamPosition};
            else
                return readResult;
        }
        else if (parentNode.isList())
            return {ConfigReadResult::NextAction::ReturnToParentNode, {}, {}};
    }
    return {ConfigReadResult::NextAction::ContinueReading, {}, {}};
}

std::optional<ConfigReadResult> parseListElementNodeSection(
        Stream& stream,
        figcone::TreeNode& parent,
        const std::string& parentName)
{
    if (!parent.isList())
        return ConfigReadResult{ConfigReadResult::NextAction::ContinueReading, {}, {}};

    stream.skip(3);
    skipWhitespace(stream, false);
    if (stream.atEnd())
        return ConfigReadResult{ConfigReadResult::NextAction::ReturnToRootNode, {}, {}};

    if (stream.peek() != "\n")
        throw ConfigError{
                "Wrong config node list '" + parentName +
                        "' format:"
                        " there can't be anything besides comments and whitespaces "
                        "on the same line with list separator '###'",
                stream.position()};

    skipWhitespace(stream, true);
    const auto readResult = [&]()->ConfigReadResult{
        if (stream.atEnd())
            return {ConfigReadResult::NextAction::ReturnToRootNode, {}, {}};
        else if (stream.peek() == "-")
            return readEndToken(stream);
        else {
            auto& nodeList = parent.asList();
            auto& newNode = nodeList.emplaceBack(stream.position());
            return parseNode(stream, newNode, parentName);
        }
    }();

    auto result = checkReadResult(readResult, parentName, parent);
    if (result.nextAction != ConfigReadResult::NextAction::ContinueReading) {
        if (result.nextAction == ConfigReadResult::NextAction::ReturnToParentNode)
            result.nextAction = ConfigReadResult::NextAction::ContinueReading;
        return result;
    }
    return {};
}

std::optional<ConfigReadResult> parseNodeSection(Stream& stream, figcone::TreeNode& parent)
{
    const auto pos = stream.position();
    const auto newNodeName = readNodeName(stream);
    if (isBlank(newNodeName))
        throw ConfigError{"Config node name can't be blank", pos};
    skipWhitespace(stream);

    if (parent.asItem().hasNode(newNodeName))
        throw ConfigError{"Config node '" + newNodeName + "' already exist", pos};
    auto& newNode = [&]() -> decltype(auto)
    {
        if (stream.peek(3) == "###")
            return parent.asItem().addNodeList(newNodeName, pos);
        else
            return parent.asItem().addNode(newNodeName, pos);
    }();

    const auto readResult = parseNode(stream, newNode, newNodeName);
    auto result = checkReadResult(readResult, newNodeName, parent);
    if (result.nextAction != ConfigReadResult::NextAction::ContinueReading) {
        if (result.nextAction == ConfigReadResult::NextAction::ReturnToParentNode)
            result.nextAction = ConfigReadResult::NextAction::ContinueReading;
        return result;
    }
    return {};
}

ConfigReadResult parseNode(Stream& stream, figcone::TreeNode& node, const std::string& nodeName)
{
    while (!stream.atEnd()) {
        const auto nextChar = stream.peek().front();
        if (std::isspace(nextChar))
            stream.skip(1);
        else if (stream.peek(3) == "###") {
            if (auto res = parseListElementNodeSection(stream, node, nodeName))
                return *res;
        }
        else if (nextChar == '#') {
            if (auto res = parseNodeSection(stream, node))
                return *res;
        }
        else if (nextChar == '-')
            return readEndToken(stream);
        else {
            const auto [paramName, param] = parseParam(stream);
            if (param.isItem())
                node.asItem().addParam(paramName, param.value());
            else
                node.asItem().addParamList(paramName, param.valueList());
        }
    }
    return {ConfigReadResult::NextAction::ReturnToRootNode, {}, {}};
}

} //namespace figcone::shoal::detail
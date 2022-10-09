#pragma once
#include "configreadresult.h"
#include <optional>

namespace figcone{
class TreeNode;
}

namespace figcone::shoal::detail {
class Stream;

std::string readNodeName(Stream& stream);
ConfigReadResult readEndToken(Stream& stream);
ConfigReadResult checkReadResult(
        const ConfigReadResult& readResult,
        const std::string& newNodeName,
        const figcone::TreeNode& parentNode);
std::optional<ConfigReadResult> parseListElementNodeSection(
        Stream& stream,
        figcone::TreeNode& parent,
        const std::string& parentName);

std::optional<ConfigReadResult> parseNodeSection(Stream& stream, figcone::TreeNode& parent);
ConfigReadResult parseNode(Stream& stream, figcone::TreeNode& node, const std::string& nodeName);\

}
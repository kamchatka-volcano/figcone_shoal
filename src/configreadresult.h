#pragma once
#include <figcone_tree/streamposition.h>
#include <string>

namespace figcone::shoal::detail {

struct ConfigReadResult {
    enum class NextAction {
        ContinueReading,
        ReturnToParentNode,
        ReturnToNodeByName,
        ReturnToRootNode
    } nextAction;
    std::string parentNodeName;
    StreamPosition returnToNodeStreamPosition;
};

} //namespace figcone::shoal::detail
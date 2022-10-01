#ifndef FIGCONE_SHOAL_CONFIGREADRESULT_H
#define FIGCONE_SHOAL_CONFIGREADRESULT_H

#include <figcone_tree/streamposition.h>
#include <string>

namespace figcone::shoal::detail {

struct ConfigReadResult{
    enum class NextAction{
        ContinueReading,
        ReturnToParentNode,
        ReturnToNodeByName,
        ReturnToRootNode
    } nextAction;
    std::string parentNodeName;
    StreamPosition returnToNodeStreamPosition;
};

}

#endif //FIGCONE_SHOAL_CONFIGREADRESULT_H
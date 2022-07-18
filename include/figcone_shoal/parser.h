#pragma once
#include "detail/nodeparser.h"
#include "detail/stream.h"
#include <figcone_tree/tree.h>
#include <figcone_tree/iparser.h>

namespace figcone::shoal{
class Parser : public IParser{
public:
    TreeNode parse(std::istream& stream) override
    {
        auto inputStream = detail::Stream{stream};
        auto rootNode = makeTreeRoot();
        detail::parseNode(inputStream, rootNode, "");
        return rootNode;
    }
};
}
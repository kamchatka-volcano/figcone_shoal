#include <figcone_shoal/parser.h>
#include "nodeparser.h"
#include "stream.h"

namespace figcone::shoal{

TreeNode Parser::parse(std::istream& stream)
{
    auto inputStream = detail::Stream{stream};
    auto rootNode = makeTreeRoot();
    detail::parseNode(inputStream, rootNode, "");
    return rootNode;
}

}

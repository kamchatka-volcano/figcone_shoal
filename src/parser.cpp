#include "nodeparser.h"
#include "stream.h"
#include <figcone_shoal/parser.h>

namespace figcone::shoal {

TreeNode Parser::parse(std::istream& stream)
{
    auto inputStream = detail::Stream{stream};
    auto rootNode = makeTreeRoot();
    detail::parseNode(inputStream, rootNode, "");
    return rootNode;
}

} //namespace figcone::shoal

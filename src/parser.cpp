#include "nodeparser.h"
#include "stream.h"
#include <figcone_shoal/parser.h>

namespace figcone::shoal {

Tree Parser::parse(std::istream& stream)
{
    auto inputStream = detail::Stream{stream};
    auto rootNode = makeTreeRoot();
    detail::parseNode(inputStream, *rootNode, "");
    return Tree{std::move(rootNode)};
}

} //namespace figcone::shoal

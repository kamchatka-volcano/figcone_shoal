#pragma once
#include <figcone_tree/tree.h>

namespace figcone::shoal::detail {
class Stream;

std::pair<std::string, figcone::TreeParam> parseParam(Stream& stream);

}
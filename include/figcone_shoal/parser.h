#ifndef FIGCONE_SHOAL_PARSER_H
#define FIGCONE_SHOAL_PARSER_H

#include <figcone_tree/iparser.h>
#include <figcone_tree/tree.h>

namespace figcone::shoal {
class Parser : public IParser {
public:
    TreeNode parse(std::istream& stream) override;
};

} //namespace figcone::shoal

#endif //FIGCONE_SHOAL_PARSER_H
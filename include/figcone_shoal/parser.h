#ifndef FIGCONE_SHOAL_PARSER_H
#define FIGCONE_SHOAL_PARSER_H

#include <figcone_tree/iparser.h>
#include <figcone_tree/stringconverter.h>
#include <figcone_tree/tree.h>

namespace figcone {
template<>
struct StringConverter<bool> {
    static std::optional<bool> fromString(const std::string& data)
    {
        if (data == "true" || data == "1")
            return true;
        if (data == "false" || data == "0")
            return false;
        return std::nullopt;
    }
};
} //namespace figcone

namespace figcone::shoal {

class Parser : public IParser {
public:
    TreeNode parse(std::istream& stream) override;
};

} //namespace figcone::shoal

#endif //FIGCONE_SHOAL_PARSER_H
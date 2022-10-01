#ifndef FIGCONE_SHOAL_UTILS_H
#define FIGCONE_SHOAL_UTILS_H

#include "stream.h"
#include "external/gsl/util"
#include <figcone_tree/errors.h>
#include <string>
#include <optional>
#include <functional>

namespace figcone::shoal::detail{
class Stream;

inline bool isSpace(char ch)
{
    return std::isspace(static_cast<unsigned char>(ch));
}

inline bool isBlank(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), [](auto ch){return isSpace(ch);});
}


inline void skipLine(Stream& stream)
{
    while(!stream.atEnd())
        if (stream.read().front() == '\n')
            return;
}

inline void skipWhitespace(Stream& stream, bool withNewLine = true)
{
    while(!stream.atEnd()) {
        auto nextChar = stream.peek().front();
        if (!withNewLine && nextChar == '\n')
            return;

        if (isSpace(nextChar))
            stream.skip(1);
        else
            return;
    }
}

inline std::string readUntil(Stream& stream, std::function<bool(char)> stopPred)
{
    auto result = std::string{};
    while (!stream.atEnd()) {
        if (stopPred(stream.peek().front()))
            return result;
        result += stream.read();
    }
    return result;
}

inline std::string readUntil(Stream& stream, const std::string& stopChars = {})
{
    return readUntil(stream,
                     [&stopChars](char ch){return stopChars.find(ch) != std::string::npos;});
}

inline std::string readWord(Stream& stream, const std::string& stopChars = {})
{
    return readUntil(stream,
                     [&stopChars](char ch) { return isSpace(ch) || stopChars.find(ch) != std::string::npos; });
}

inline std::optional<std::string> readQuotedString(Stream& stream)
{
    if (stream.atEnd())
        return {};

    auto quotationMark = stream.peek().front();
    if (quotationMark != '\'' && quotationMark != '"')
        return {};

    stream.skipComments(false);
    const auto restoreSkipOnExit = gsl::final_action([&stream]{ stream.skipComments(true); });
    auto pos = stream.position();
    stream.skip(1);

    auto result = std::string{};
    while (!stream.atEnd()) {
        auto ch = stream.read().front();
        if (ch == quotationMark)
            return result;
        result += ch;
    }
    throw ConfigError{"String isn't closed", pos};
}

}

#endif //FIGCONE_SHOAL_UTILS_H
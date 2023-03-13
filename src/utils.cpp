#include "utils.h"
#include "stream.h"
#include <figcone_tree/errors.h>
#include <sfun/string_utils.h>
#include <gsl/util>
#include <algorithm>

namespace figcone::shoal::detail {

bool isBlank(const std::string& str)
{
    return std::all_of(str.begin(), str.end(), sfun::isspace);
}

void skipLine(Stream& stream)
{
    while (!stream.atEnd())
        if (stream.read().front() == '\n')
            return;
}

void skipWhitespace(Stream& stream, bool withNewLine)
{
    while (!stream.atEnd()) {
        auto nextChar = stream.peek().front();
        if (!withNewLine && nextChar == '\n')
            return;

        if (sfun::isspace(nextChar))
            stream.skip(1);
        else
            return;
    }
}

std::string readUntil(Stream& stream, std::function<bool(char)> stopPred)
{
    auto result = std::string{};
    while (!stream.atEnd()) {
        if (stopPred(stream.peek().front()))
            return result;
        result += stream.read();
    }
    return result;
}

std::string readUntil(Stream& stream, const std::string& stopChars)
{
    return readUntil(
            stream,
            [&stopChars](char ch)
            {
                return stopChars.find(ch) != std::string::npos;
            });
}

std::string readWord(Stream& stream, const std::string& stopChars)
{
    return readUntil(
            stream,
            [&stopChars](char ch)
            {
                return sfun::isspace(ch) || stopChars.find(ch) != std::string::npos;
            });
}

std::optional<std::string> readQuotedString(Stream& stream)
{
    if (stream.atEnd())
        return {};

    auto quotationMark = stream.peek().front();
    if (quotationMark != '\'' && quotationMark != '"' && quotationMark != '`')
        return {};

    stream.skipComments(false);
    const auto restoreSkipOnExit = gsl::final_action(
            [&stream]
            {
                stream.skipComments(true);
            });
    auto pos = stream.position();
    stream.skip(1);

    if (stream.peek() == "\n")
        stream.skipLineSeparator();

    auto result = std::string{};
    while (!stream.atEnd()) {
        auto ch = stream.read().front();
        if (ch == quotationMark)
            return result;
        result += ch;
    }
    throw ConfigError{"String isn't closed", pos};
}

} //namespace figcone::shoal::detail
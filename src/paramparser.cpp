#include "paramparser.h"
#include "stream.h"
#include "utils.h"
#include <figcone_tree/errors.h>
#include <gsl/util>
#include <optional>
#include <utility>

namespace figcone::shoal::detail {

namespace {

void skipParamWhitespace(Stream& stream, const std::string& paramName)
{
    skipWhitespace(stream, false);
    if (stream.peek() == "\n")
        throw ConfigError{
                "Wrong param '" + paramName + "' format: parameter's value must be placed on the same line as its name",
                stream.position()};
}

std::optional<std::string> readSingleParam(
        Stream& stream,
        const std::string& wordSeparator,
        const std::vector<std::string>& paramListValue,
        const std::string& paramName,
        bool isMultiline)
{
    if (isMultiline)
        stream.skipComments(false);
    const auto restoreSkipOnExit = gsl::final_action(
            [&]
            {
                if (isMultiline)
                    stream.skipComments(true);
            });

    auto quotedParam = readQuotedString(stream);
    if (quotedParam)
        return *quotedParam;
    else {
        auto result = readWord(stream, wordSeparator);
        if (result.empty()) {
            if (stream.peek() == "," || (paramListValue.empty() && !isMultiline))
                throw ConfigError{"Parameter list '" + paramName + "' element is missing", stream.position()};
            if (paramListValue.empty() && isMultiline)
                return {};
        }
        return result;
    }
}

figcone::TreeParam makeParam(
        const std::vector<std::string>& paramValueList,
        const StreamPosition& position,
        bool isList)
{
    if (isList)
        return figcone::TreeParam{paramValueList, position};
    else
        return figcone::TreeParam{paramValueList.at(0), position};
}

figcone::TreeParam readParamOrParamList(
        Stream& stream,
        const std::string& paramName,
        StreamPosition pos,
        bool isMultiline = false)
{
    auto paramValueList = std::vector<std::string>{};
    auto isList = isMultiline;
    while (!stream.atEnd()) {
        auto paramValue = readSingleParam(stream, isMultiline ? ",]" : ",", paramValueList, paramName, isMultiline);
        if (paramValue)
            paramValueList.emplace_back(std::move(*paramValue));

        skipWhitespace(stream, isMultiline);
        const auto endOfList = isMultiline ? "]" : "\n";
        if (stream.peek() == ",") {
            isList = true;
            stream.skip(1);
            skipWhitespace(stream, isMultiline);
            if (stream.peek() == endOfList || stream.atEnd())
                throw ConfigError{"Parameter list '" + paramName + "' element is missing", stream.position()};
        }
        else if (stream.peek() == endOfList) {
            stream.skip(1);
            return makeParam(paramValueList, pos, isList);
        }
        else if (stream.atEnd())
            return makeParam(paramValueList, pos, isList);
        else
            throw ConfigError{
                    "Wrong param '" + paramName + "' format: there must be only one parameter per line",
                    stream.position()};
    }
    return makeParam(paramValueList, pos, isList);
}

figcone::TreeParam readParamValue(Stream& stream, const std::string& paramName, StreamPosition pos)
{
    skipWhitespace(stream, false);
    if (stream.peek() == "\n" || stream.atEnd())
        throw ConfigError{"Parameter '" + paramName + "' value is missing", stream.position()};

    if (stream.peek() == "[") {
        stream.skip(1);
        skipWhitespace(stream);
        return readParamOrParamList(stream, paramName, pos, true);
    }
    else
        return readParamOrParamList(stream, paramName, pos, false);
}

} //namespace

std::pair<std::string, figcone::TreeParam> parseParam(Stream& stream)
{
    skipWhitespace(stream);
    auto paramPos = stream.position();
    auto paramName = readWord(stream, "=");
    if (paramName.empty())
        throw ConfigError{"Parameter's name can't be empty", paramPos};

    skipParamWhitespace(stream, paramName);

    auto pos = stream.position();
    if (stream.read() != "=")
        throw ConfigError{"Wrong param '" + paramName + "' format: missing '='", pos};

    skipParamWhitespace(stream, paramName);
    return {paramName, readParamValue(stream, paramName, paramPos)};
}

} //namespace figcone::shoal::detail
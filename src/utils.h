#pragma once
#include <string>
#include <optional>
#include <functional>

namespace figcone::shoal::detail{
class Stream;

bool isSpace(char ch);
bool isBlank(const std::string& str);
void skipLine(Stream& stream);
void skipWhitespace(Stream& stream, bool withNewLine = true);
std::string readUntil(Stream& stream, std::function<bool(char)> stopPred);
std::string readUntil(Stream& stream, const std::string& stopChars = {});
std::string readWord(Stream& stream, const std::string& stopChars = {});
std::optional<std::string> readQuotedString(Stream& stream);

}
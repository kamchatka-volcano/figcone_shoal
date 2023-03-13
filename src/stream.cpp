#include "stream.h"

namespace figcone::shoal::detail {

Stream::Stream(std::istream& stream, const StreamPosition& startPosition)
    : stream_(stream)
    , startPosition_(startPosition)
{
}

void Stream::skip(int size)
{
    read(size);
}

void Stream::skipLineSeparator()
{
    auto ch = char{};
    auto pos = stream_.tellg();
    if (!stream_.get(ch))
        return;
    if (ch == '\n')
        return;
    else if (ch == '\r') {
        pos = stream_.tellg();
        if (!stream_.get(ch))
            return;
        if (ch != '\n')
            stream_.seekg(pos);
    }
    else
        stream_.seekg(pos);
}

void Stream::skipComments(bool state)
{
    skipComments_ = state;
}

std::string Stream::read(int size)
{
    auto result = std::string{};
    auto ch = char{};
    for (auto i = 0; i < size; ++i) {
        if (!stream_.get(ch))
            return {};
        if (skipComments_ && ch == ';') {
            skipLine();
            continue;
        }
        if (ch == '\r' && peek() == "\n") {
            i--;
            continue;
        }
        else if (ch == '\r' || ch == '\n') {
            (*position_.line)++;
            (*position_.column) = 0;
            ch = '\n';
        }
        else if (ch == '\t')
            (*position_.column) += 4;
        else
            (*position_.column)++;
        result.push_back(ch);
    }
    return result;
}

std::string Stream::peek(int size)
{
    auto result = std::string{};
    auto ch = char{};
    auto pos = stream_.tellg();
    for (auto i = 0; i < size; ++i) {
        if (!stream_.get(ch)) {
            stream_.clear();
            result.clear();
            break;
        }
        if (skipComments_ && ch == ';') {
            skipLine();
            i--;
        }
        else if (ch == '\r') {
            auto crPos = stream_.tellg();
            if (stream_.get(ch)) {
                if (ch != '\n')
                    stream_.seekg(crPos);
            }
            else
                stream_.clear();

            result.push_back('\n');
        }
        else
            result.push_back(ch);
    }
    stream_.seekg(pos);
    return result;
}

bool Stream::atEnd()
{
    return peek().empty();
}

StreamPosition Stream::position() const
{
    return {*startPosition_.line + *position_.line, *startPosition_.column + *position_.column};
}

void Stream::skipLine()
{
    auto ch = char{};
    auto prevPos = stream_.tellg();
    while (stream_.get(ch)) {
        if (ch == '\r' || ch == '\n') {
            stream_.seekg(prevPos);
            break;
        }
        prevPos = stream_.tellg();
    }
}

} //namespace figcone::shoal::detail
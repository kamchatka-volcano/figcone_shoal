#pragma once
#include <figcone_tree/streamposition.h>
#include <istream>
#include <string>

namespace figcone::shoal::detail {

class Stream {
public:
    explicit Stream(std::istream& stream, const StreamPosition& startPosition = StreamPosition{1, 1});
    Stream(Stream&&) = default;
    Stream(const Stream&) = delete;
    Stream& operator=(const Stream&) = delete;

    void skip(int size);
    void skipLineSeparator();
    void skipComments(bool state);
    std::string read(int size = 1);
    std::string peek(int size = 1);
    bool atEnd();
    StreamPosition position() const;

private:
    void skipLine();

private:
    std::istream& stream_;
    StreamPosition position_ = {0, 0};
    StreamPosition startPosition_ = {0, 0};
    bool skipComments_ = true;
};

} //namespace figcone::shoal::detail

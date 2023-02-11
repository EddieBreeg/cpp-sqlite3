#include <SQLite3/Blob.hpp>

namespace SQLite3
{
    constexpr Blob::Blob(nullptr_t) noexcept {}
    Blob::Blob(const uint8_t *data, size_t n) noexcept: _start(data), _end(data + n) {}
    Blob::Blob(const uint8_t *data, const uint8_t *end) noexcept: _start(data), _end(end) {}

    
} // namespace SQLite3

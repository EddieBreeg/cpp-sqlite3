#pragma once

#include <cinttypes>
#include <cstddef>
#include <iterator>

namespace SQLite3
{
    /* A read-only view over a simple buffer of bytes
    */
    class Blob
    {
    private:
        const uint8_t *_start = nullptr, *_end = nullptr;
    public:
        using pointer = const uint8_t*;
        using iterator = pointer;
        constexpr Blob() noexcept = default;
        constexpr Blob(nullptr_t) noexcept;
        Blob(const uint8_t* start, const uint8_t *end) noexcept;
        Blob(const uint8_t *start, size_t end) noexcept;

        const uint8_t& operator[](size_t i) const {return _start[i]; }

        inline iterator begin() const noexcept { return _start; }
        inline iterator end() const noexcept { return _end; }

        inline size_t size() const noexcept { return _end - _start; }
        
        inline pointer data() const noexcept { return _start; }
        inline bool is_empty() const noexcept { return _start == _end; } 

        ~Blob() = default;
    };
    /* Utility class, which allows you to bind a blob filled with zeros to a statement parameter */
    class ZeroBlob {
        size_t _size = 0;
        static constexpr uint8_t val = 0;
    public:
        constexpr ZeroBlob() = default;
        ZeroBlob(size_t n): _size(n) {}
        class iterator {
            size_t i = 0;
        public:
            using value_type = uint8_t;
            using difference_type = size_t;
            using pointer = const uint8_t*;
            using iterator_category = std::random_access_iterator_tag;
            using reference = const uint8_t&;

            iterator(size_t i): i(i) {}
            constexpr reference operator*() const noexcept { return ZeroBlob::val; }
            constexpr reference operator[](size_t n) const noexcept { return ZeroBlob::val; } 
            constexpr pointer operator->() const noexcept { return &ZeroBlob::val; }

            bool operator==(const iterator& other) const noexcept { return i == other.i; }
            bool operator!=(const iterator& other) const noexcept { return i != other.i; }
            bool operator<(const iterator& other) const noexcept { return i < other.i; }
            bool operator<=(const iterator& other) const noexcept { return *this == other || *this < other; }
            bool operator>=(const iterator& other) const noexcept { return !(*this < other); }
            bool operator>(const iterator& other) const noexcept { return i > other.i; }

            iterator operator+(difference_type n) const { return iterator(i + n); }
            iterator operator-(difference_type n) const { return iterator(i - n); }
            iterator& operator+=(difference_type n) { i += n; return *this; }
            iterator& operator-=(difference_type n) { i -= n; return *this; }
            iterator& operator++() { ++i; return *this; }
            iterator& operator--() { --i; return *this; }
            iterator operator++(int) { auto tmp = i; ++i; return iterator(tmp); }
            iterator operator--(int) { auto tmp = i; --i; return iterator(tmp); }
        };
        // the argument here is ignored: return value is always 0
        constexpr const uint8_t& operator[](size_t) const noexcept { return val; }
        iterator begin() const noexcept { return iterator(0); }
        iterator end() const noexcept { return iterator(_size); }
        bool is_empty() const noexcept { return _size == 0; }
        size_t size() const noexcept {return _size;}
        constexpr nullptr_t data() const { return nullptr; }
    };
    
} // namespace SQLite3

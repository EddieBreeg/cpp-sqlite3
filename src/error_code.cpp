#include <SQLite3/error_code.hpp>
#include <SQLite3/internal/sqlite3.h>

#define SWITCH_CASE(expr)   case expr: _msg = #expr; break

namespace SQLite3
{
    error_code::error_code(SQLite3Error val, const char *msg): _value(val), _msg(msg) {}
    error_code::error_code(SQLite3Error value): _value(value), _msg(sqlite3_errstr(int(value)))
    {}
    bool error_code::operator==(SQLite3Error code) const { return _value == code; }
    bool error_code::operator!=(SQLite3Error code) const { return _value != code; }
} // namespace SQLite3

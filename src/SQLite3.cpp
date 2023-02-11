#include <SQLite3/SQLite3.hpp>
#include <SQLite3/internal/sqlite3.h>

namespace SQLite3
{
    error_code init(){
        return error_code{(SQLite3Error)sqlite3_initialize()};
    }
    error_code shutdown() { return error_code{(SQLite3Error)sqlite3_shutdown()}; }
} // namespace SQLite3

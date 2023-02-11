#include <SQLite3/Database.hpp>
#include <SQLite3/internal/sqlite3.h>
#include <SQLite3/Statement.hpp>

namespace SQLite3
{
    Database::Database(std::string_view path, bool extCodes){
        int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        flags |= extCodes * SQLITE_OPEN_EXRESCODE;
        int ec = sqlite3_open_v2(path.data(), &_db, flags, nullptr);
        if(ec) {
            _status = false;
            return;
        }
        sqlite3_extended_result_codes(_db, extCodes);
    }
    bool Database::isOpen() const {
        return _status;
    }
    error_code Database::getError() const {
        int ec = sqlite3_errcode(_db);
        const char *msg = sqlite3_errmsg(_db);
        return {SQLite3::SQLite3Error(ec), msg};
    }
    error_code Database::toggleExtendedErrorCodes(bool onoff){
        int ec = sqlite3_extended_result_codes(_db, onoff);
        return ec? 
            error_code(SQLite3Error(ec), sqlite3_errmsg(_db)) :
            error_code {};
    }
    Database::~Database(){
        sqlite3_close_v2(_db);
    }
    Database::Database(Database&& other): _db(other._db){
        other._db = nullptr;
    }
    Database& Database::operator=(Database&& other){
        std::swap(_db, other._db);
        return *this;
    }
    Statement Database::createStatement(std::string_view sql) {
        return Statement(_db, sql);
    }
    Statement Database::createStatement(std::string_view sql, error_code& ec) {
        return Statement(_db, sql, ec);
    }
    error_code Database::toggleForeignKeys(bool onoff){
        #if defined(SQLITE_OMIT_FOREIGN_KEY) || SQLITE_OMIT_TRIGGER
        static_assert(0, "Foreign keys are not supported");
        #endif
        return execute("PRAGMA foreign_keys = ON;");
    }

    error_code Database::execute(std::string_view sql) {
        int ec = sqlite3_exec(_db, sql.data(), nullptr, nullptr, nullptr);
        if(!ec) return error_code(SQLite3Error::OK);
        error_code r(SQLite3Error(ec), sqlite3_errmsg(_db));
        return r;
    }
} // namespace SQLite3

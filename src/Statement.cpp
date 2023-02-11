#include <SQLite3/Statement.hpp>
#include <SQLite3/internal/sqlite3.h>

namespace SQLite3
{
    
    Statement::Statement(sqlite3 *db,std::string_view sql): _db(db)
    {
        int ec = sqlite3_prepare_v2(db, sql.data(), sql.length(), &_s, nullptr);
        if(ec)
            throw error_code(SQLite3Error(ec), sqlite3_errmsg(db));
    }
    Statement::Statement(sqlite3 *db,std::string_view sql, error_code &err): _db(db)
    {
        int ec = sqlite3_prepare_v2(db, sql.data(), sql.length(), &_s, nullptr);
        err = ec? error_code(SQLite3Error(ec), sqlite3_errmsg(db)) : error_code{};
    }
    Statement::Statement(sqlite3 *db,std::string_view sql, std::string_view& tail, 
        error_code& err): _db(db)
    {
        const char *p = nullptr;
        int ec = sqlite3_prepare_v2(db, sql.data(), sql.length(), &_s, &p);
        tail = sql.substr(p - sql.data());
        err = ec? error_code(SQLite3Error(ec), sqlite3_errmsg(db)) : error_code{};
    }

    error_code Statement::bindParam(int index, std::string_view str){
        int ec = sqlite3_bind_text64(_s, index, str.data(), str.length(), nullptr, SQLITE_UTF8);
        if(ec != SQLITE_ERROR) return error_code(SQLite3Error(ec));
        return error_code(SQLite3Error(ec), sqlite3_errmsg(_db));
    }
    error_code Statement::bindParam(int index, Blob blob){
        int ec = sqlite3_bind_blob64(_s, index, blob.data(), blob.size(), nullptr);
        if(ec != SQLITE_ERROR) return error_code(SQLite3Error(ec));
        return error_code(SQLite3Error(ec), sqlite3_errmsg(_db));
    }
    error_code Statement::bindParam(int index, ZeroBlob blob){
        int ec = sqlite3_bind_zeroblob64(_s, index, blob.size());
        if(ec != SQLITE_ERROR) return error_code(SQLite3Error(ec));
        return error_code(SQLite3Error(ec), sqlite3_errmsg(_db));
    }
    error_code Statement::bindParam(int index, nullptr_t){
        int ec = sqlite3_bind_null(_s, index);
        if(ec != SQLITE_ERROR) return error_code(SQLite3Error(ec));
        return error_code(SQLite3Error(ec), sqlite3_errmsg(_db));
    }
    error_code Statement::bindParam(int index, int32_t value){
        int ec = sqlite3_bind_int(_s, index, value);
        if(ec != SQLITE_ERROR) return error_code(SQLite3Error(ec));
        return error_code(SQLite3Error(ec), sqlite3_errmsg(_db));
    }
    error_code Statement::bindParam(int index, int64_t value){
        int ec = sqlite3_bind_int64(_s, index, value);
        if(ec != SQLITE_ERROR) return error_code(SQLite3Error(ec));
        return error_code(SQLite3Error(ec), sqlite3_errmsg(_db));
    }
    error_code Statement::bindParam(int index, double value){
        int ec = sqlite3_bind_double(_s, index, value);
        if(ec != SQLITE_ERROR) return error_code(SQLite3Error(ec));
        return error_code(SQLite3Error(ec), sqlite3_errmsg(_db));
    }
    QueryResult Statement::operator()() {
        sqlite3_step(_s);
        return QueryResult(_s);
    }
    QueryResult Statement::operator()(error_code& err) {
        int ec = sqlite3_step(_s);
        err = error_code(SQLite3Error(ec), sqlite3_errmsg(_db));
        return QueryResult(_s);
    }

    Statement::Statement(Statement&& other): _db(other._db), _s(other._s) {
        other._s = nullptr;
    }
    Statement& Statement::operator=(Statement&& other){
        std::swap(_s, other._s);
        _db = other._db;
        return *this;
    }
    Statement::~Statement(){
        if(_s) sqlite3_finalize(_s);
    }

    int QueryResult::_col_int(int index)const{
        return sqlite3_column_int(_s, index);
    }
    double QueryResult::_col_double(int index)const{
        return sqlite3_column_double(_s, index);
    }
    int64_t QueryResult::_col_int64(int index)const{
        return sqlite3_column_int64(_s, index);
    }
    std::string_view QueryResult::_col_text(int index)const{
        size_t len = sqlite3_column_bytes(_s, index);
        return {(const char*)sqlite3_column_text(_s, index), len};
    }
    Blob QueryResult::_col_blob(int index) const{
        size_t n = sqlite3_column_bytes(_s, index);
        return Blob((const uint8_t*)sqlite3_column_blob(_s, index), n);
    }

    QueryResult::QueryResult(sqlite3_stmt* st): _s(st), _cols(sqlite3_column_count(st))
    {}
} // namespace SQLite3


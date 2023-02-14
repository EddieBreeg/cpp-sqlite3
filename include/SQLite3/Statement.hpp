#pragma once
#include <string_view>

#include "error_code.hpp"
#include "Blob.hpp"
#include <stdexcept>


struct sqlite3_stmt;
struct sqlite3;


namespace SQLite3
{
    /* An abstracted representation of the current row returned by a SQL statement */
    class QueryResult {
        sqlite3_stmt *_s;
        int _cols = 0;
        int _col_int(int index) const;
        int64_t _col_int64(int index) const;
        double _col_double(int index) const;
        std::string_view _col_text(int index) const;
        Blob _col_blob(int index) const;
    public:
        /* Constructs the object from a raw sqlite3_stmt object. This is used internally and shouldn't be used 
        directly */
        QueryResult(sqlite3_stmt* statement);
        /* The number of columns in the row. Might be 0 if the statement didn't return any data. */
        int columns() const {return _cols; }
        /* Returns the value of a column, cast to T
        @tparam T: The type of value returned from the column. If the actual value type contained in the column
        does not match, an automatic cast will be performed
        @param index: The 0-based index of the column to retrieve the value from
        @return The value stored in the column if it exists, converted to T. 
         */
        template<typename T>
        T at(int index) const;
    };
    /* Retrieves the value from a column, converted to a 32-bit signed integer.
    @param index: The 0-based column index 
    @return: The value stored in the column if it exists, converted to an integer
    */
    template<>
    inline int32_t QueryResult::at<int32_t>(int index) const {
        return _col_int(index);
    }
    /* Retrieves the value from a column, converted to a 64-bit signed integer.
    @param index: The 0-based column index 
    @return: The value stored in the column if it exists, converted to an integer
    */
    template<>
    inline int64_t QueryResult::at<int64_t>(int index) const {
        return _col_int64(index);
    }
    /* Retrieves the value from a column, converted to a UTF-8 encoded string.
    @param index: The 0-based column index 
    @return: The value stored in the column if it exists, converted to a read-only string
    */
    template<>
    inline std::string_view QueryResult::at<std::string_view>(int index) const {
        return _col_text(index);
    }
    /* Retrieves the value from a column, converted to a double.
    @param index: The 0-based column index 
    @return: The value stored in the column if it exists, converted to a double
    */
    template<>
    inline double QueryResult::at<double>(int index) const {
        return _col_double(index);
    }
    /* Retrieves the value from a column, converted to a blob.
    @param index: The 0-based column index 
    @return: The value stored in the column if it exists, converted to a blob
    */
    template<>
    inline Blob QueryResult::at<Blob>(int index) const {
        return _col_blob(index);
    }
    /* Abstract representation of a single SQL statement */
    class Statement
    {
    private:
        sqlite3_stmt* _s = nullptr;
        sqlite3* _db = nullptr;

        template<typename T>
        error_code _bind_params_impl(int index, const T& val){
            return bindParam(index, val);
        }
        template<typename T, typename... Args>
        error_code _bind_params_impl(int index, const T& first, const Args... others){
            auto rc = bindParam(index, first);
            if(rc == SQLite3Error::ERROR) return rc;
            return _bind_params_impl(index+1, others...);
        }
    public:
        /* Binds an UTF-8 encoded string to a statement parameter
        @param index: The 1-based index of the parameter in the prepared statement
        @param str: The value to bind
         */
        error_code bindParam(int index, std::string_view str);
        /* Binds a blob of binary data to a statement parameter
        @param index: The 1-based index of the parameter in the prepared statement
        @param str: The value to bind
         */
        error_code bindParam(int index, Blob blob);
        /* Binds a blob of 0s to a statement parameter
        @param index: The 1-based index of the parameter in the prepared statement
        @param blob: The value to bind
         */
        error_code bindParam(int index, ZeroBlob blob);
        /* Binds a 32-bit integer to a statement parameter
        @param index: The 1-based index of the parameter in the prepared statement
        @param blob: The value to bind
         */
        error_code bindParam(int index, int32_t value);
        /* Binds a 64-bit integer to a statement parameter
        @param index: The 1-based index of the parameter in the prepared statement
        @param value: The value to bind
         */
        error_code bindParam(int index, int64_t value);
        /* Binds a double precision float to a statement parameter
        @param index: The 1-based index of the parameter in the prepared statement
        @param value: The value to bind
         */
        error_code bindParam(int index, double value);
        /* Binds NULL to a statement parameter
        @param index: The 1-based index of the parameter in the prepared statement
         */
        error_code bindParam(int index, nullptr_t);

        /* Binds a list of statement parameters. The first value will be bound to the first parameter in the statement,
        and so on.
        @tparam Args: The types of values to bind
        @param values: The values to bind */
        template<typename... Args>
        error_code bindParams(const Args&... values){
            return _bind_params_impl(1, values...);
        }
        /* Executes the SQL statement once, and returns the result corresponding result object 
        @return A QueryResult representing the row returned by the statement. If the statement did not 
        return any actual data row, the result will have 0 column.
        */
        QueryResult operator()();
        /* Executes the SQL statement once, and returns the result corresponding result object.
        The error code indicating the status of the statement will be stored in ec.
        @param ec: The error code which indicates how the operation turned out. If an error occurred,
        ec will contain the appropriate code. If a row of data has been returned, ec will be equal to 
        SQLite3::ROW. If the statement terminated successfully and no additional row is available, ec will
        be equal to SQLite3::DONE.
        @return A QueryResult representing the row returned by the statement. If the statement did not 
        return any actual data row, the result will have 0 column.
        */
        QueryResult operator()(error_code& ec);

        /* Indicates whether the object is an empty statement, which does nothing (a SQL comment being a typical case) */
        bool empty() const { return !bool(_s); }

        
        /* Constructs the object from a raw database connection object and the sql source code.
        Is used internally and should not be called directly: use Database::createStatement to create 
        a new Statement object */
        Statement(sqlite3* db, std::string_view sqlSource);
        /* Constructs the object from a raw database connection object and the sql source code.
        Is used internally and should not be called directly: use Database::createStatement to create 
        a new Statement object */
        Statement(sqlite3* db, std::string_view sqlSource, error_code& ec);
        /* Constructs the object from a raw database connection object and the sql source code.
        Is used internally and should not be called directly: use Database::createStatement to create 
        a new Statement object */
        Statement(sqlite3* db, std::string_view sql, std::string_view& tail, error_code& ec);
        Statement(const Statement&) = delete;
        Statement(Statement&& other);
        Statement() = default;
        Statement& operator=(const Statement&) = delete;
        Statement& operator=(Statement&& other);
        ~Statement();
    };
    
} // namespace SQLite3


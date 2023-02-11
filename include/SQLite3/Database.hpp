#pragma once

#include <string_view>
#include "Statement.hpp"
#include "error_code.hpp"
#include <vector>

struct sqlite3;

namespace SQLite3
{
    /* Represents a connection to a SQLite3 database*/
    class Database
    {
    private:
        sqlite3* _db = nullptr;
        bool _status = true;
    public:
        /* 
        Creates the connection to the database file.
        If the connection fails for some reason, any subsequent call to isOpen() will return false,
        and getError() will return the code describing the specific reason for the failure 
        @param path: the path to the database file
        @param extended_error_codes: whether to activate extended error codes for function return values

         */
        Database(std::string_view path, bool extended_error_codes = true);
        Database(const Database&) = delete;
        Database(Database&& other);
        /*
        Indicates if the connection is active
        */
        bool isOpen() const;
        /* Returns the error code for the most recent API call */
        error_code getError() const;
        Database& operator=(const Database&) = delete;
        Database& operator=(Database&& other);
        /* toggle the extended error codes feature
        */
        error_code toggleExtendedErrorCodes(bool onoff);
        /* Creates a sql statement from some sql code.
        @param sql: the sql source code to compile
        @throws Throws an instance of SQLite3::error_code on failure
        @return On success, returns the newly created sql statement 
        */
        Statement createStatement(std::string_view sql);
        /* Creates a sql statement from some sql code, and stores the call result in ec 
        @param sql: the source code to compile
        @param ec: a reference to an error_code object to store the result of the operation
        */
        Statement createStatement(std::string_view sql, error_code& ec);
        /* Executes one or more sql statements directly. Should ONLY be used with remade sql statements.
        When statement parameters are required, use prepared statement (created with createStatement) instead.*/
        error_code execute(std::string_view sql);
        /* Toggles foreign keys constraints (which DISABLED BY DEFAULT for historical compatibility purposes)
         */
        error_code toggleForeignKeys(bool onoff);
        /* Executes one or more sql statements, and calls the provided callback function for each row returned by said statements. If a statement fails to compile or to execute, the appropriate error code is returned without calling the callback. If the callback returns an error_code which is not SQLite3::OK, SQLite3::ABORT is returned immediately
        @tparam Cbk: The type of function object
        @tparam Args: The argument types for the callback
        @param sql: the sql source code to execute
        @param callback: a function object to call for each row returned by an executed statement. The callback is expected to take in a QueryResult& as the first argument, and to return an error_code which signals if the call was successful. 
        @param args: a list of arbitrary arguments to forward to the callback object
         */
        template<class Cbk, typename... Args>
        error_code execute(std::string_view sql, Cbk&& callback, 
            Args... args)
        {
            while (!sql.empty())
            {
                error_code ec;
                Statement s(_db, sql, sql, ec);
                if(ec) return ec;
                if(s.empty()) // empty statement (eg: comments)
                    continue;
                while(1){
                    auto res = s(ec);
                    if(ec == SQLite3Error::DONE)
                        break;
                    else if(ec != SQLite3Error::ROW)
                        return ec;
                    error_code ec2 = callback(res, args...);
                    if(ec2 != SQLite3Error::OK) 
                        return SQLite3Error::ABORT;
                }
            }
            return error_code{};
        }
        ~Database();
    };

    
    
} // namespace SQLite3


#pragma once

#include <string>

namespace SQLite3
{
    /* All the supported SQLite error codes */
    enum  SQLite3Error: unsigned {
        OK=0,
        ERROR,
        INTERNAL,
        PERM,
        ABORT,
        BUSY,
        LOCKED,
        NOMEM,
        READONLY,
        INTERRUPT,
        IOERR,
        CORRUPT,
        NOTFOUND,
        FULL,
        CANTOPEN,
        PROTOCOL,
        EMPTY,
        SCHEMA,
        TOOBIG,
        CONSTRAINT,
        MISMATCH,
        MISUSE,
        NOLFS,
        AUTH,
        FORMAT,
        RANGE,
        NOTADB,
        NOTICE,
        WARNING,
        ROW     =   100,
        DONE    =   101,
        ERROR_MISSING_COLLSEQ   = (ERROR | (1<<8)),
        ERROR_RETRY             = (ERROR | (2<<8)),
        ERROR_SNAPSHOT          = (ERROR | (3<<8)),
        IOERR_READ              = (IOERR | (1<<8)),
        IOERR_SHORT_READ        = (IOERR | (2<<8)),
        IOERR_WRITE             = (IOERR | (3<<8)),
        IOERR_FSYNC             = (IOERR | (4<<8)),
        IOERR_DIR_FSYNC         = (IOERR | (5<<8)),
        IOERR_TRUNCATE          = (IOERR | (6<<8)),
        IOERR_FSTAT             = (IOERR | (7<<8)),
        IOERR_UNLOCK            = (IOERR | (8<<8)),
        IOERR_RDLOCK            = (IOERR | (9<<8)),
        IOERR_DELETE            = (IOERR | (10<<8)),
        IOERR_BLOCKED           = (IOERR | (11<<8)),
        IOERR_NOMEM             = (IOERR | (12<<8)),
        IOERR_ACCESS            = (IOERR | (13<<8)),
        IOERR_CHECKRESERVEDLOCK = (IOERR | (14<<8)),
        IOERR_LOCK              = (IOERR | (15<<8)),
        IOERR_CLOSE             = (IOERR | (16<<8)),
        IOERR_DIR_CLOSE         = (IOERR | (17<<8)),
        IOERR_SHMOPEN           = (IOERR | (18<<8)),
        IOERR_SHMSIZE           = (IOERR | (19<<8)),
        IOERR_SHMLOCK           = (IOERR | (20<<8)),
        IOERR_SHMMAP            = (IOERR | (21<<8)),
        IOERR_SEEK              = (IOERR | (22<<8)),
        IOERR_DELETE_NOENT      = (IOERR | (23<<8)),
        IOERR_MMAP              = (IOERR | (24<<8)),
        IOERR_GETTEMPPATH       = (IOERR | (25<<8)),
        IOERR_CONVPATH          = (IOERR | (26<<8)),
        IOERR_VNODE             = (IOERR | (27<<8)),
        IOERR_AUTH              = (IOERR | (28<<8)),
        IOERR_BEGIN_ATOMIC      = (IOERR | (29<<8)),
        IOERR_COMMIT_ATOMIC     = (IOERR | (30<<8)),
        IOERR_ROLLBACK_ATOMIC   = (IOERR | (31<<8)),
        IOERR_DATA              = (IOERR | (32<<8)),
        IOERR_CORRUPTFS         = (IOERR | (33<<8)),
        LOCKED_SHAREDCACHE      = (LOCKED |  (1<<8)),
        LOCKED_VTAB             = (LOCKED |  (2<<8)),
        BUSY_RECOVERY           = (BUSY   |  (1<<8)),
        BUSY_SNAPSHOT           = (BUSY   |  (2<<8)),
        BUSY_TIMEOUT            = (BUSY   |  (3<<8)),
        CANTOPEN_NOTEMPDIR      = (CANTOPEN | (1<<8)),
        CANTOPEN_ISDIR          = (CANTOPEN | (2<<8)),
        CANTOPEN_FULLPATH       = (CANTOPEN | (3<<8)),
        CANTOPEN_CONVPATH       = (CANTOPEN | (4<<8)),
        CANTOPEN_DIRTYWAL       = (CANTOPEN | (5<<8)) /* Not Used */,
        CANTOPEN_SYMLINK        = (CANTOPEN | (6<<8)),
        CORRUPT_VTAB            = (CORRUPT | (1<<8)),
        CORRUPT_SEQUENCE        = (CORRUPT | (2<<8)),
        CORRUPT_INDEX           = (CORRUPT | (3<<8)),
        READONLY_RECOVERY       = (READONLY | (1<<8)),
        READONLY_CANTLOCK       = (READONLY | (2<<8)),
        READONLY_ROLLBACK       = (READONLY | (3<<8)),
        READONLY_DBMOVED        = (READONLY | (4<<8)),
        READONLY_CANTINIT       = (READONLY | (5<<8)),
        READONLY_DIRECTORY      = (READONLY | (6<<8)),
        ABORT_ROLLBACK          = (ABORT | (2<<8)),
        CONSTRAINT_CHECK        = (CONSTRAINT | (1<<8)),
        CONSTRAINT_COMMITHOOK   = (CONSTRAINT | (2<<8)),
        CONSTRAINT_FOREIGNKEY   = (CONSTRAINT | (3<<8)),
        CONSTRAINT_FUNCTION     = (CONSTRAINT | (4<<8)),
        CONSTRAINT_NOTNULL      = (CONSTRAINT | (5<<8)),
        CONSTRAINT_PRIMARYKEY   = (CONSTRAINT | (6<<8)),
        CONSTRAINT_TRIGGER      = (CONSTRAINT | (7<<8)),
        CONSTRAINT_UNIQUE       = (CONSTRAINT | (8<<8)),
        CONSTRAINT_VTAB         = (CONSTRAINT | (9<<8)),
        CONSTRAINT_ROWID        = (CONSTRAINT |(10<<8)),
        CONSTRAINT_PINNED       = (CONSTRAINT |(11<<8)),
        CONSTRAINT_DATATYPE     = (CONSTRAINT |(12<<8)),
        NOTICE_RECOVER_WAL      = (NOTICE | (1<<8)),
        NOTICE_RECOVER_ROLLBACK = (NOTICE | (2<<8)),
        WARNING_AUTOINDEX       = (WARNING | (1<<8)),
        AUTH_USER               = (AUTH | (1<<8)),
        OK_LOAD_PERMANENTLY     = (OK | (1<<8)),
    };
    // Represents an error code and the corresponding English UTF-8 encoded message
    class error_code
    {
    private:
        SQLite3Error _value;
        const char* _msg;
    public:
        error_code(SQLite3Error value, const char *msg);
        error_code(SQLite3Error value = SQLite3Error::OK);
        error_code(const error_code& other) = default;
        inline operator bool() const { return bool(_value); }
        /* @return An English UTF-8 encoded string describing the message */
        inline const char *what() const throw() { return _msg; }
        inline SQLite3Error value() const { return _value; }
        bool operator==(SQLite3Error code) const;
        bool operator!=(SQLite3Error code) const;
        ~error_code() = default;
    };
    
    
} // namespace SQLite3

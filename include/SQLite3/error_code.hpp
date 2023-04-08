#pragma once

#include <string>

namespace SQLite3
{
/* All the supported SQLite error codes */
    enum  class SQLite3Error: unsigned {
        Ok=0,
        Error,
        Internal,
        Perm,
        Abort,
        Busy,
        Locked,
        Nomem,
        ReadOnly,
        Interrupt,
        IOErr,
        Corrupt,
        Notfound,
        Full,
        Cantopen,
        Protocol,
        Empty,
        Schema,
        Toobig,
        Constraint,
        Mismatch,
        Misuse,
        Nolfs,
        Auth,
        Format,
        Range,
        Notadb,
        Notice,
        Warning,
        Row     =   100,
        Done    =   101,
        MissingCollseq   = (Error | (1<<8)),
        Retry             = (Error | (2<<8)),
        Snapshot          = (Error | (3<<8)),
        IOErrRead              = (IOErr | (1<<8)),
        IOErrShortRead        = (IOErr | (2<<8)),
        IOErrWrite             = (IOErr | (3<<8)),
        IOErrFsync             = (IOErr | (4<<8)),
        IOErrDirFsync         = (IOErr | (5<<8)),
        IOErrTruncate          = (IOErr | (6<<8)),
        IOErrFstat             = (IOErr | (7<<8)),
        IOErrUnlock            = (IOErr | (8<<8)),
        IOErrRdlock            = (IOErr | (9<<8)),
        IOErrDelete            = (IOErr | (10<<8)),
        IOErrBlocked           = (IOErr | (11<<8)),
        IOErrNomem             = (IOErr | (12<<8)),
        IOErrAccess            = (IOErr | (13<<8)),
        IOErrCheckreservedlock = (IOErr | (14<<8)),
        IOErrLock              = (IOErr | (15<<8)),
        IOErrClose             = (IOErr | (16<<8)),
        IOErrDirClose         = (IOErr | (17<<8)),
        IOErrShmopen           = (IOErr | (18<<8)),
        IOErrShmsize           = (IOErr | (19<<8)),
        IOErrShmlock           = (IOErr | (20<<8)),
        IOErrShmmap            = (IOErr | (21<<8)),
        IOErrSeek              = (IOErr | (22<<8)),
        IOErrDeleteNoent      = (IOErr | (23<<8)),
        IOErrMmap              = (IOErr | (24<<8)),
        IOErrGettemppath       = (IOErr | (25<<8)),
        IOErrConvpath          = (IOErr | (26<<8)),
        IOErrVnode             = (IOErr | (27<<8)),
        IOErrAuth              = (IOErr | (28<<8)),
        IOErrBeginAtomic      = (IOErr | (29<<8)),
        IOErrCommitAtomic     = (IOErr | (30<<8)),
        IOErrRollbackAtomic   = (IOErr | (31<<8)),
        IOErrData              = (IOErr | (32<<8)),
        IOErrCorruptfs         = (IOErr | (33<<8)),
        LockedSharedcache      = (Locked |  (1<<8)),
        LockedVtab             = (Locked |  (2<<8)),
        BusyRecovery           = (Busy   |  (1<<8)),
        BusySnapshot           = (Busy   |  (2<<8)),
        BusyTimeout            = (Busy   |  (3<<8)),
        CantopenNotempdir      = (Cantopen | (1<<8)),
        CantopenIsdir          = (Cantopen | (2<<8)),
        CantopenFullpath       = (Cantopen | (3<<8)),
        CantopenConvpath       = (Cantopen | (4<<8)),
        CantopenDirtywal       = (Cantopen | (5<<8)) /* Not Used */,
        CantopenSymlink        = (Cantopen | (6<<8)),
        CorruptVtab            = (Corrupt | (1<<8)),
        CorruptSequence        = (Corrupt | (2<<8)),
        CorruptIndex           = (Corrupt | (3<<8)),
        ReadOnlyRecovery       = (ReadOnly | (1<<8)),
        ReadOnlyCantlock       = (ReadOnly | (2<<8)),
        ReadOnlyRollback       = (ReadOnly | (3<<8)),
        ReadOnlyDbmoved        = (ReadOnly | (4<<8)),
        ReadOnlyCantinit       = (ReadOnly | (5<<8)),
        ReadOnlyDirectory      = (ReadOnly | (6<<8)),
        AbortRollback          = (Abort | (2<<8)),
        ConstraintCheck        = (Constraint | (1<<8)),
        ConstraintCommithook   = (Constraint | (2<<8)),
        ConstraintForeignkey   = (Constraint | (3<<8)),
        ConstraintFunction     = (Constraint | (4<<8)),
        ConstraintNotnull      = (Constraint | (5<<8)),
        ConstraintPrimarykey   = (Constraint | (6<<8)),
        ConstraintTrigger      = (Constraint | (7<<8)),
        ConstraintUnique       = (Constraint | (8<<8)),
        ConstraintVtab         = (Constraint | (9<<8)),
        ConstraintRowid        = (Constraint |(10<<8)),
        ConstraintPinned       = (Constraint |(11<<8)),
        ConstraintDatatype     = (Constraint |(12<<8)),
        NoticeRecoverWal      = (Notice | (1<<8)),
        NoticeRecoverRollback = (Notice | (2<<8)),
        WarningAutoindex       = (Warning | (1<<8)),
        AuthUser               = (Auth | (1<<8)),
        OkLoadPermanently     = (Ok | (1<<8)),
    };
    // Represents an error code and the corresponding English UTF-8 encoded message
    class error_code
    {
    private:
        SQLite3Error _value;
        const char* _msg;
    public:
        error_code(SQLite3Error value, const char *msg);
        error_code(SQLite3Error value = SQLite3Error::Ok);
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

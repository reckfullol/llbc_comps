#pragma once

#include <functional>
#include "llbc/common/StringDataType.h"

using namespace llbc;

enum class MODE
{
    MODE_NONE,
    MODE_READ,  //read only
    MODE_EDIT   //read and write
};

/**
* Database single record object.
*/
class IRecord
{
public:
    virtual ~IRecord() = default;

    virtual sint64 GetInt(uint32 idx) const = 0;
    virtual double GetDouble(uint32 idx) const = 0;
    virtual const LLBC_String &GetStr(uint32 idx) const = 0;

    virtual sint64 GetInt(const LLBC_String& name) const = 0;
    virtual double GetDouble(const LLBC_String& name) const = 0;
    virtual const LLBC_String &GetStr(const LLBC_String& name) const = 0;

public:
    virtual void SetInt(uint32 idx, sint64 val) = 0;
    virtual void SetDouble(uint32 idx, double val) = 0;
    virtual void SetStr(uint32 idx, const char *val) = 0;
    virtual void SetBlob(uint32 nIdx, const char *val, uint32 len) = 0;

    virtual void SetInt(const LLBC_String& name, sint64 val) = 0;
    virtual void SetDouble(const LLBC_String& name, double val) = 0;
    virtual void SetStr(const LLBC_String& name, const char *val) = 0;
    virtual void SetBlob(const LLBC_String& name, const char *val, uint32 len) = 0;
};

/**
 * Database records set
 */
class IRecordset
{
public:
    virtual ~IRecordset() = default;

    // Get a record from set. (not release record)
    virtual IRecord *GetRecord(uint32 idx) = 0;

    //Pop a record from set.(release record)
    virtual IRecord *PopRecord(uint32 idx) = 0;

    //Get record count.
    virtual uint32 GetSize() const = 0;

    // Get a record ref.
    virtual IRecord &operator[](uint32 idx) = 0;
};

/**
 * async query callback.
 */
using AsyncQueryCB = std::function<void(IRecordset *)>;
/**
* async execute sql callback.
*/
using AsyncSqlCB = std::function<void(bool)>;

/**
 * Database object.
 */
class IDatabase
{
public:
    virtual ~IDatabase() = default;

    // Init database.
    virtual bool Init(const LLBC_String &ip, 
                      int port, 
                      const LLBC_String &user, 
                      const LLBC_String &passwd, 
                      const LLBC_String &dbName,
                      int asyncConnNum) = 0;

    // destroy database.
    virtual void Destroy() = 0;

    // flush all async query.
    virtual void Flush() = 0;

public:
    // synchronize query.
    virtual IRecordset *Query(const char *sql, MODE mode) = 0;

    // synchronize execute sql.
    virtual bool Query(const char *sql) = 0;

    // asynchronous query.
    virtual void QueryAsync(uint64 key, const char *sql, MODE mode, AsyncQueryCB cb) = 0;

    // asynchronous execute sql.
    virtual void QueryAsync(uint64 key, const char *sql, AsyncSqlCB cb) = 0;
};

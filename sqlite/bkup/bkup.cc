#include <iostream>
#include <iomanip>
#include <sqlite3.h>
#include <string>
#include <sstream>
#include <stdexcept>
#include <functional>
#include <cassert>

using namespace std;

struct Message {
    char *ptr;

    Message()
    : ptr(nullptr)
    {
    }

    virtual ~Message()
    {
        sqlite3_free(ptr);
    }

    operator bool() const
    {
        return ptr;
    }
};

ostream& operator<<(ostream &stream, const Message msg)
{
    stream << msg.ptr;
    return stream;
}

class DataBase {
    sqlite3 *db_;
    sqlite3_backup *backup_;

    bool isResultOk(const int rc, const Message &msg = Message()) const
    {
        if (rc == SQLITE_OK)
            return true;
        cerr << "SQlite3 API error: code: " << rc;
        if (msg)
            cerr << ", " << msg;
        cerr << endl;
        return false;
    }

public:
    DataBase()
    : db_(nullptr),
      backup_(nullptr)
    {
    }

    virtual ~DataBase()
    {
        sqlite3_close(db_);
    }

    bool open(const string &filename)
    {
        const int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
        const int rc = sqlite3_open_v2(filename.c_str(), &db_, flags, nullptr);
        return isResultOk(rc);
    }

    bool exec(const string sql)
    {
        Message errMsg;
        const int rc = sqlite3_exec(db_, sql.c_str(),
                                    nullptr, nullptr, &errMsg.ptr);
        return isResultOk(rc, errMsg);
    }

    bool begin()
    {
        return exec("begin");
    }

    bool commit()
    {
        return exec("commit");
    }

    bool createTable(bool if_not_exists=false)
    {
        string sql = "create table";
        if (if_not_exists)
            sql += " if not exists";
        sql += " t1(number)";
        return exec(sql);
    }

    bool insert(long num)
    {
        stringstream ss;
        ss << "insert into t1 values(" << num << ")";
        return exec(ss.str().c_str());
    }

    bool backupInit(DataBase &src)
    {
        assert(db_);
        backup_ = sqlite3_backup_init(db_, "main", src.db_, "main");
        if (!backup_)
            cerr << "Failed to call sqlite3_backup_init()" << endl;
        return backup_;
    }

    bool backupStep(function<bool(int, int)> callback)
    {
        assert(backup_);
        bool done = false;
        while (!done) {
            const int rc = sqlite3_backup_step(backup_, 1);
            if (rc == SQLITE_DONE)
                done = true;
            else if (rc == SQLITE_BUSY)
                cout << "backup: BUSY" << endl;
            else if (rc == SQLITE_LOCKED)
                cout << "backup: LOCKED" << endl;
            else if (!isResultOk(rc))
                return false;

            if (!callback(sqlite3_backup_remaining(backup_),
                          sqlite3_backup_pagecount(backup_)))
                return false;
        }
        return true;
    }

    bool backupFinish()
    {
        const int rc = sqlite3_backup_finish(backup_);
        return isResultOk(rc);
    }
};

static long pow2(size_t n)
{
    long v = 1;
    for (size_t i = 0; i < n; i++)
        v *= 2;
    return v;
}

static void checker(const bool result, int lineno)
{
    if (result)
        return;

    stringstream ss;
    ss << "Stop at line: " << lineno;
    throw runtime_error(ss.str());
}

#define DO(R) checker(R, __LINE__)

static string generateUpdateStatement(size_t idx)
{
    stringstream ss;
    const long num = ((3L << idx) * 0x100L) | (0xfc);
    const auto rowid = idx;
    cout << "Update: rowid: " << rowid << ", num: "
      << setfill('0') << setw(8) << right << hex << num
      << dec << endl;
    ss << "update t1 set number=" << num << " where rowid=" << rowid;
    return ss.str();
}

static void start()
{
    const auto filename = "test1.db";
    DataBase db;
    DO(db.open(filename));
    DO(db.exec("pragma page_size = 512"));
    DO(db.createTable());

    DO(db.begin());
    for (size_t i = 0; i < 63; i++) {
        const long n = pow2(i) - 2;
        DO(db.insert(n));
    }
    DO(db.commit());

    DataBase bk;
    const auto bkupname = "backup1.db";
    DO(bk.open(bkupname));
    DO(bk.exec("pragma page_size = 512"));
    DO(bk.backupInit(db));
    size_t idx = 0;
    DO(bk.backupStep([&](int remaining, int count) {
        cout << "backup: Remaining: " << remaining <<
                 ", count: " << count << endl;
        if (remaining == 0)
            return true;
        return db.exec(generateUpdateStatement(idx++));
    }));
    DO(bk.backupFinish());
}

int main(int argc, char *argv[])
{
    try {
        start();
    } catch (...) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

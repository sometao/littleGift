#pragma once
#include "seeker/logger.h"
#include "sqlite3.h"
#include <string>
#include <functional>

namespace seeker {
using std::function;
using std::string;

class SqliteDB {
  const string dbFile;

  sqlite3* dbHandle = nullptr;

  SqliteDB(const string& file) : dbFile(file) {
    int rc = sqlite3_open(dbFile.c_str(), &dbHandle);
    I_LOG(
        "sqlite3_open file[{}] "
        "result={}",
        dbFile, rc);
    if (rc) {
      E_LOG(
          "Can't open "
          "database[{}]: {}",
          dbFile, sqlite3_errmsg(dbHandle));
      sqlite3_close(dbHandle);
      throw std::runtime_error("open database error.");
    }
    I_LOG("Database opened: {}", dbFile);
  }

  static SqliteDB& getInstence(const string& file = "") {
    static SqliteDB instence{file};
    return instence;
  }

 public:
  SqliteDB(const SqliteDB&) = delete;
  SqliteDB& operator=(const SqliteDB&) = delete;

  ~SqliteDB() {
    std::cout << "Database destructed begin" << std::endl;
    W_LOG("Database destructed begin.");
    if (dbHandle) {
      W_LOG("Database destructed do nothing.");
      // sqlite3_close(dbHandle);
    }
    W_LOG("Database destructed end.");
    std::cout << "Database destructed end" << std::endl;
  }

  static void init(const string& file) {
    static bool inited = false;
    if (!inited) {
      inited = true;
      I_LOG("Init Database with file = ", file);
      getInstence();
      I_LOG("Database inited success. db file = {}", file);

    } else {
      W_LOG("Database has been inited before, do nothing.");
    }
  }

  static int exec(const string& sql,
                  int (*callback)(void*, int, char**, char**),
                  void* arg1,
                  string& err) {
    auto& ins = getInstence();
    T_LOG("exec sql: {}.", sql);
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(ins.dbHandle, sql.c_str(), callback, arg1, &zErrMsg);
    if (zErrMsg != nullptr) {
      err = zErrMsg;
    }
    T_LOG("exec rc=[{}] sql : {}.", rc, sql);
    return rc;
  }
};

}  // namespace seeker
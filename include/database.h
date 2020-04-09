#pragma once
#include "littleGift.h"
#include "sqlite3.h"
#include <string>

namespace database {
using std::string;

struct SlideRow {
  int64_t id;
  string authorName;
  string content;
  string contentType;
  string accessToken;
  string editCode;
  int64_t create_time;
};

class DB {
  const string dbFile;

  sqlite3* dbHandle = nullptr;

  void initContentTable();

  DB::DB();

 public:
  DB(const DB&) = delete;

  DB& operator=(const DB&) = delete;

  ~DB();

  static DB& getInstence();

  void init();

  // TODO implement this.
  bool insert(const SlideRow& row);
};

}  // namespace database
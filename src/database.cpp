#include "database.h"

namespace database {

DB::DB() : dbFile(SQLITE_DB_FILE) {
  int rc = sqlite3_open(dbFile.c_str(), &dbHandle);
  T_LOG("sqlite3_open result={}", rc);
  if (rc) {
    E_LOG("Can't open database[{}]: {}", dbFile, sqlite3_errmsg(dbHandle));
    sqlite3_close(dbHandle);
    throw std::runtime_error("open database error.");
  }
  I_LOG("Database opened: {}", dbFile);
}

void DB::init() {
  I_LOG("Database init: begin", dbFile);

  I_LOG("Database init: end", dbFile);
}

void DB::initContentTable() {
  {
    const string sql = R"(
CREATE TABLE IF NOT EXISTS slides(
	id						INTEGER					PRIMARY KEY AUTOINCREMENT,
	author_name		VARCHAR(64),
	content				TEXT						NOT NULL,	
	content_type	VARCHAR(64),	
	access_token	VARCHAR(128)		NOT NULL,
	edit_code			VARCHAR(128)		NOT NULL,
	create_time		INTEGER					NOT NULL );
)";

    char* zErrMsg = 0;
    int rc = sqlite3_exec(dbHandle, sql.c_str(), nullptr, 0, &zErrMsg);
    if (rc != SQLITE_OK) {
      E_LOG("SQL createContentTable error: {}", zErrMsg);
      sqlite3_free(zErrMsg);
      throw std::runtime_error("SQL createContentTable error");
    }
    I_LOG("init createContentTable done.");
  }
}

DB::~DB() {
  W_LOG("Database destructed begin.");
  //if (dbHandle) {
  //  W_LOG("Database destructed do nothing.");
  //  // sqlite3_close(dbHandle);
  //}
  //W_LOG("Database destructed end.");
}

DB& DB::getInstence() {
  static DB instence{};
  return instence;
}

}  // namespace database

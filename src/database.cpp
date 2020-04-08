#include "littleGift.h"
#include <string>
#include <functional>
#include "sqlite3.h"
#include "memory"

namespace database {
using std::function;
using std::string;
using std::unique_ptr;

const string dbFile = SQLITE_DB_FILE;

static void shutdown(sqlite3* db) {
  if (db) {
    sqlite3_close(db);
    I_LOG("Database[{}] shutdown.", dbFile);
  } else {
    E_LOG("Can't shutdown database[{}], it is nullptr", dbFile);
  }
}

static unique_ptr<sqlite3, function<void(sqlite3*)>> dbHandle{nullptr, shutdown};

namespace {

void initContentTable() { 
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
  int rc = sqlite3_exec(dbHandle.get(), sql.c_str(), nullptr, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    E_LOG("SQL createContentTable error: {}", zErrMsg);
    sqlite3_free(zErrMsg);
    throw std::runtime_error("SQL createContentTable error");
  }
  I_LOG("init createContentTable.");
}

}  // namespace


//TODO test db init.
void init() {
  sqlite3* db;
  int rc = sqlite3_open(dbFile.c_str(), &db);
  T_LOG("sqlite3_open result={}", rc);
  if (rc) {
    E_LOG("Can't open database[{}]: {}", dbFile, sqlite3_errmsg(db));
    sqlite3_close(db);
    throw std::runtime_error("open database error.");
  }
  dbHandle.reset(db);
  initContentTable();
}

}  // namespace database

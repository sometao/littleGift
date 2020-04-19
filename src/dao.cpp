#include "littleGift.h"
#include "seeker/database.h"
#include "dao.h"
#include <iostream>
#include <string>
#include <memory>

namespace littleGift {

namespace dao {

using seeker::SqliteDB;
using std::cout;
using std::endl;
using std::shared_ptr;
using std::string;


static int callback1(void* NotUsed, int argc, char** argv, char** azColName) {
  int i;
  cout << "runSql callback rst: ";
  for (i = 0; i < argc; i++) {
    cout << azColName[i] << "=[" << (argv[i] ? argv[i] : "NULL") << "]   ";
  }
  cout << endl;
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}

int64_t addSlides(const SlidesRow& row) {
  static const string sql =
      "insert into slides (author_name, content, content_type, access_token, edit_code, "
      "create_time ) values (?, ?, ?, ?, ?, ?);";

  D_LOG("addSlides author_name={}, content={} ", row.authorName, row.content.substr(0, 6));
  auto stmtPtr = SqliteDB::perpare(sql);
  auto stmt = stmtPtr.get();

  int rc[6];
  rc[0] = sqlite3_bind_text(stmt, 1, row.authorName.c_str(), -1, SQLITE_STATIC);
  rc[1] = sqlite3_bind_text(stmt, 2, row.content.c_str(), -1, SQLITE_STATIC);
  rc[2] = sqlite3_bind_text(stmt, 3, row.contentType.c_str(), -1, SQLITE_STATIC);
  rc[3] = sqlite3_bind_text(stmt, 4, row.accessToken.c_str(), -1, SQLITE_STATIC);
  rc[4] = sqlite3_bind_text(stmt, 5, row.editCode.c_str(), -1, SQLITE_STATIC);
  rc[5] = sqlite3_bind_int64(stmt, 6, row.createTime);

  for (int i = 0; i < 6; i++) {
    if (rc[i] != SQLITE_OK) {
      std::string msg = fmt::format("bind index[{}] errorCode: {}", (i + 1), rc[i]);
      E_LOG(msg);
      throw std::runtime_error(msg);
    }
  }

  int64_t id = -1;
  {
    auto& m = SqliteDB::getInsertMutex();
    std::lock_guard<std::mutex> lock(m);
    int rc1 = sqlite3_step(stmt);
    if (rc1 != SQLITE_DONE) {
      std::string msg = fmt::format("addSlides sqlite3_step error[{}] SQL", rc1);
      E_LOG(msg);
      throw std::runtime_error(msg);
    }
    id = SqliteDB::lastInsertRowid();
  }
  return id;
}

//Return nullptr if can not get gift by the token.
shared_ptr<SlidesRow> getSlides(const string& token) {
  static const string sql =
      "select id, author_name, content, content_type, access_token, edit_code, create_time "
      "from slides where access_token = ?;";

  D_LOG("getSlides token={} ", token);

  auto stmtPtr = SqliteDB::perpare(sql);
  auto stmt = stmtPtr.get();

  if (sqlite3_bind_text(stmt, 1, token.c_str(), -1, SQLITE_STATIC) != SQLITE_OK) {
    std::string msg = fmt::format("bind errorCode: token={}", token);
    E_LOG(msg);
    throw std::runtime_error(msg);
  }

  int rc = sqlite3_step(stmt);
  if (rc == SQLITE_ROW) {
    shared_ptr<SlidesRow> row = std::make_shared<SlidesRow>();

    row->id = sqlite3_column_int64(stmt, 0);
    row->authorName = (char*)sqlite3_column_text(stmt, 1);
    row->content = (char*)sqlite3_column_text(stmt, 2);
    row->contentType = (char*)sqlite3_column_text(stmt, 3);
    row->accessToken = (char*)sqlite3_column_text(stmt, 4);
    row->editCode = (char*)sqlite3_column_text(stmt, 5);
    row->createTime = sqlite3_column_int64(stmt, 6);
    return row;

  } else if (rc == SQLITE_DONE) {
    W_LOG("can not getSlides with token={}", token);
    shared_ptr<SlidesRow> empty{};
    return empty;
  } else {
    std::string msg = fmt::format("getSlides sqlite3_step error[{}] SQL", rc);
    E_LOG(msg);
    throw std::runtime_error(msg);
  }

}

//TODO editCode rename to authCode?
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
CREATE UNIQUE INDEX IF NOT EXISTS slides_tkn_idx on slides (access_token);
)";

  string err{};
  int rc = SqliteDB::exec(sql.c_str(), callback1, 0, err);
  if (rc != SQLITE_OK) {
    E_LOG("SQL createContentTable error: {}", err);
    throw std::runtime_error("SQL createContentTable error");
  }
  I_LOG("init createContentTable done.");
}


void init() {
  seeker::SqliteDB::init( SQLITE_DB_FILE );
  initContentTable();
}


}  // namespace dao



}  // namespace littleGift

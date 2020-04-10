#include "littleGift.h"
#include "DAO.h"
#include "seeker/database.h"
#include <iostream>
#include <string>
namespace littleGift {
using seeker::SqliteDB;
using std::cout;
using std::endl;
using std::string;


namespace dao {



int callback1(void* NotUsed, int argc, char** argv, char** azColName) {
  int i;
  cout << "runSql callback rst: ";
  for (i = 0; i < argc; i++) {
    cout << azColName[i] << "=[" << (argv[i] ? argv[i] : "NULL") << "]   ";
  }
  cout << endl;
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}



void initContentTable() {
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

    string err{};
    int rc = SqliteDB::exec(sql.c_str(), callback1, 0, err);
    if (rc != SQLITE_OK) {
      E_LOG("SQL createContentTable error: {}", err);
      throw std::runtime_error("SQL createContentTable error");
    }
    I_LOG("init createContentTable done.");
  }
}





}  // namespace dao



}  // namespace littleGift

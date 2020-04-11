#include "littleGift.h"
#include "seeker/database.h"
#include "DAO.h"
#include <string>
#include <iostream>

namespace littleGift {
namespace dao {

  void initContentTable();
  int64_t addSlides(SlidesRow row);

}


namespace test {



void testDbInit() {
  D_LOG("test SqliteDB begin.");
  using namespace seeker;
  SqliteDB::init(SQLITE_DB_FILE);
  D_LOG("test SqliteDB done.");
}

void testAddSlides() {
  D_LOG("test SqliteDB begin.");
  using namespace seeker;
  SqliteDB::init(SQLITE_DB_FILE);
  void initContentTable();
  D_LOG("test SqliteDB done.");
}

}  // namespace test

}  // namespace littleGift

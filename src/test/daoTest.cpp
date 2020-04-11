#include "littleGift.h"
#include "seeker/database.h"
#include "DAO.h"
#include <string>
#include <iostream>

namespace littleGift {

namespace test {

using namespace littleGift::dao;


void testDbInit() {
  D_LOG("test SqliteDB begin.");
  using namespace seeker;
  SqliteDB::init(SQLITE_DB_FILE);
  D_LOG("test SqliteDB done.");
}

void testAddSlides() {
  D_LOG("test testAddSlides begin.");
  using namespace seeker;
  SqliteDB::init(SQLITE_DB_FILE);

  initContentTable();

  SlidesRow r{-1, "Tom", "hello content.", "type1", "token001", "code002", 1235l};

  auto id = addSlides(r);
  I_LOG("add slides done, id = {}", id );

  D_LOG("test testAddSlides done.");
}

}  // namespace test

}  // namespace littleGift

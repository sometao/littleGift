#include "littleGift.h"
#include "seeker/database.h"
#include <iostream>
#include <string>
#include <atomic>
#include <chrono>
#include <thread>
#include "httpUtils.h"
#include "htmlTemplate.h"

using std::cout;
using std::endl;
using std::string;

namespace littleGift {
extern void setRoutes(httplib::Server& server);
extern void config(httplib::Server& server);
}  // namespace littleGift

//namespace logger {
//extern void setBaseLogger(bool offStdOut = false, bool offFileOut = false);
//extern void shutdownLogger();
//}  // namespace logger

namespace lgtest {
extern void testHtmlTemplate();
extern void testStringSplit();
extern void testHttpClient();
}  // namespace lgtest

void testDB() {
  D_LOG("test SqliteDB begin.");
  using namespace seeker;
  SqliteDB::init( SQLITE_DB_FILE );

  D_LOG("test SqliteDB done.");
}

int main() {
  // testHtmlTemplate();
  // lgtest::testHttpClient();

  seeker::Logger::init( LOG_FILE_NAME );

  E_LOG(" --  [1]   -----------------");
  testDB();


  I_LOG(" --  [2]   -----------------");

  //I_LOG(" --  [10]   -----------------");
  //std::this_thread::sleep_for(std::chrono::milliseconds(1500));

  cout << "DONE" << endl;
  return 0;
}

namespace {
using namespace httplib;

void startServer(Server& svr, const char* host, int port) {
  I_LOG("starting server {}:{}", host, port);
  svr.listen(host, port, 0);
  W_LOG("Server thread exited.");
}

}  // namespace

int main1() {
  using namespace httplib;

  cout << "STARTED." << endl;

  auto interface = "localhost";
  auto port = 50080;

  Server svr;
  littleGift::setRoutes(svr);
  littleGift::config(svr);
  std::thread serverThread{startServer, std::ref(svr), interface, port};
  serverThread.detach();

  int counter = 60;
  while (!svr.is_running() && --counter > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  I_LOG("SERVER STARTED: {}:{}", interface, port);

  while (svr.is_running()) {
    std::this_thread::sleep_for(std::chrono::seconds(5));
    T_LOG("Server is running");
  }

  W_LOG("SERVER STOPPED.");
  cout << "DONE." << endl;
  return 0;
}

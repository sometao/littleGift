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

namespace test {
extern void testHtmlTemplate();
extern void testStringSplit();
extern void testHttpClient();
extern void testDbInit();
extern void testAddSlides();
extern void hashTest();
extern void testRandomInt();
extern void testRandomeStr();
}  // namespace test


}  // namespace littleGift



namespace {
using namespace httplib;

void runTest() {

  try {
    //for test.
    I_LOG("runTest begin.");
    using namespace littleGift::test;
    testRandomeStr();
    I_LOG("runTest end.");

  } catch (std::runtime_error ex) {
    W_LOG("RUNTIME ERROR: {}", ex.what());
  }

}

void startServer(Server& svr, const char* host, int port) {
  I_LOG("starting server {}:{}", host, port);
  try {
  svr.listen(host, port, 0);
  } catch (std::runtime_error ex) {
    E_LOG("startServer runtime_error: {}", ex.what());
  } catch(...) {
    E_LOG("startServer unknown error.");
  }
  W_LOG("Server thread exited.");
}

int launch() {

  using namespace httplib;
  cout << "STARTED." << endl;

  auto interface = "0.0.0.0";
  auto port = 50080;

  Server svr;
  littleGift::setRoutes(svr);
  littleGift::config(svr);
  std::thread serverThread{ startServer, std::ref(svr), interface, port };
  serverThread.detach();

  int counter = 60;
  while (!svr.is_running() && --counter > 0) {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }

  I_LOG("SERVER STARTED: {}:{}", interface, port);

  while (svr.is_running()) {
    std::this_thread::sleep_for(std::chrono::seconds(30));
    T_LOG("Server is running");
  }

  W_LOG("SERVER STOPPED.");
  cout << "DONE." << endl;
  return 0;
}



}  // namespace




int main() {
  seeker::Logger::init(LOG_FILE_NAME, false);
  launch();

  //runTest();
}


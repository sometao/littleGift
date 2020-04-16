#include "littleGift.h"
#include "seeker/logger.h"
#include <iostream>
#include <string>

using std::cout;
using std::endl;
using std::string;

namespace littleGift {

void startServer(const string& interface, int port);

namespace dao {
extern void init();
}


namespace test {
// extern void testHtmlTemplate();
// extern void testStringSplit();
// extern void testHttpClient();
// extern void testDbInit();
// extern void testAddSlides();
// extern void hashTest();
// extern void testRandomInt();
// extern void testRandomeStr();
}  // namespace test


}  // namespace littleGift



namespace {

// void runTest() {
//
//  try {
//    //for test.
//    I_LOG("runTest begin.");
//    using namespace littleGift::test;
//    hashTest();
//    I_LOG("runTest end.");
//
//  } catch (std::runtime_error ex) {
//    W_LOG("RUNTIME ERROR: {}", ex.what());
//  }
//
//}

void launch() {

  littleGift::dao::init();

  auto interface = "0.0.0.0";
  auto port = 50082;

  littleGift::startServer(interface, port);

}


}  // namespace



int main() {
  seeker::Logger::init(LOG_FILE_NAME, false);
  launch();

  // runTest();
}

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

// TODO read port from input.
// TODO print version command
// TODO print version on starting.
void launch(int port = 50082) {
  littleGift::dao::init();

  auto interface = "0.0.0.0";

  littleGift::startServer(interface, port);
}

}  // namespace


int main(int argc, char* argv[]) {
  int port{-1};
  if (argc != 2) {
    cout << "Input Error." << endl;
    cout << "Usage: ./littleGift PORT" << endl;
    return -1;
  } else {
    try {
      port = std::stoi(argv[1]);
    } catch(std::invalid_argument ex) {
      cout << "Input error:" << ex.what() << endl;
      cout << "Usage: ./littleGift PORT" << endl;
      return -1;
    }  catch (...) {
      cout << "Input error." << endl;
      cout << "Usage: ./littleGift PORT" << endl;
      return -1;
    }
  }

  seeker::Logger::init(LOG_FILE_NAME, false);
  launch(port);
  return 0;


  // runTest();
}

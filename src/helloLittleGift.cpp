// helloLibevent.cpp : Defines the entry point for the application.
//
#include "httplib.h"
#include <iostream>

using std::cout;
using std::endl;
using std::string;

int main() {
  using namespace httplib;

  std::cout << "Hello LittleGift.." << std::endl;
  Server svr;

  svr.Get("/hi", [](const Request& /*req*/, Response& res) {
    res.set_content("Hello LittleGift.\n", "text/plain");
  });

  svr.listen("localhost", 8080);

  cout << "DONE." << endl;

  return 0;
}

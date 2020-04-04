#include "pch.h"
#include "httplib.h"
#include "httpUtils.h"
#include <iostream>
#include <string>
#include "htmlTemplate.h"

using std::cout;
using std::endl;
using std::string;

namespace littleGift {
extern void setRoutes(httplib::Server& server);
}

namespace lgtest {

extern void testHtmlTemplate();
extern void testStringSplit();
}  // namespace lgtest

int main1() {
  // testHtmlTemplate();
  lgtest::testStringSplit();
  return 0;
}

namespace logger {
  extern void setBaseLogger();
  extern void shutdownLogger();
}


int main() {
  using namespace httplib;

  logger::setBaseLogger();

  for (int i = 0; i < 1000; i++) {

    T_LOG("00000---------- {}", i++);
    D_LOG("1111111-------- {}", i++);
    I_LOG("222222222-------{}", i++);
    W_LOG("33333333333-----{}", i++);
    E_LOG("4444444444444---{}", i++);

  }


  cout << "STARTED." << endl;

  Server svr;

  svr.set_error_handler([](const auto& req, auto& res) {
    auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");
  });

  littleGift::setRoutes(svr);

  svr.listen("localhost", 50080);

  cout << "DONE." << endl;

  logger::shutdownLogger();
  return 0;
}

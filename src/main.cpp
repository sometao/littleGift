#include "pch.h"
#include "httplib.h"
#include "httpUtils.h"
#include <iostream>
#include <string>
#include "htmlTemplate.h"
#include <atomic>

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
  extern void setBaseLogger(bool offStdOut = false, bool offFileOut = false);
  extern void shutdownLogger();
}


int main() {
  using namespace httplib;

  logger::setBaseLogger();

  std::atomic<int> c{0};

  for (int i = 0; i < 1000; i++) {

    T_LOG("00000-----------{}", c.fetch_add(1));
    D_LOG("1111111---------{}", c.fetch_add(1));
    I_LOG("222222222-------{}", c.fetch_add(1));
    W_LOG("33333333333-----{}", c.fetch_add(1));
    E_LOG("4444444444444---{}", c.fetch_add(1));

  }


  cout << "STARTED." << endl;

  if (true) {
    logger::shutdownLogger();
    return 0;
  }



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

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

extern void testHtmlTemplate();
extern void testStringSplit();

int main1() {
  //testHtmlTemplate();
  testStringSplit();
  return 0;
}

int main() {
  using namespace httplib;

  spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
  spdlog::debug("This message should be displayed..");

  spdlog::info("hello, little Gift.");

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

  return 0;
}

#include "httplib.h"
#include <iostream>
#include <string>
#include <cstring>

using std::cout;
using std::endl;
using std::string;

extern void setRoutes(httplib::Server& server);

int main() {
  using namespace httplib;

  std::cout << "Hello LittleGift.." << std::endl;
  Server svr;

  svr.set_error_handler([](const auto& req, auto& res) {
    auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");
  });


  setRoutes(svr);


  svr.listen("localhost", 50080);

  cout << "DONE." << endl;

  return 0;
}

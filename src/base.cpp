#include "littleGift.h"
#include "httplib.h"
#include <string>
namespace littleGift {
using namespace httplib;

Server::Handler errorHandler = [](const Request& req, Response& res) {
  //auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";

  const std::string page = fmt::format(R"(
<p>Error Status: <span style='color:red;'>{}</span></p>
<a href="javascript:history.go(-1);">Back</a>
<a href="/littleGift">home</a>
)", res.status);

  W_LOG("Request Error: [{}] {} stateCode: {}", req.method, req.path, res.status);
  res.set_content(page.c_str(), "text/html");
};

httplib::Logger baseLogger = [](const Request& req, const Response& res) {
  I_LOG("[{}] access {}", req.method, req.path);
};

void config(httplib::Server& server) {
  //server.set_logger(baseLogger);
  server.set_error_handler(errorHandler);
}

}  // namespace littleGift
#include "littleGift.h"
#include "httplib.h"

namespace littleGift {
using namespace httplib;

Server::Handler errorHandler = [](const Request& req, Response& res) {
  auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
  char buf[BUFSIZ];
  snprintf(buf, sizeof(buf), fmt, res.status);
  W_LOG("Request Error: [{}] {} stateCode: {}", req.method, req.path, res.status);
  res.set_content(buf, "text/html");
};

Logger baseLogger = [](const Request& req, const Response& res) {
  I_LOG("[{}] access {}", req.method, req.path);
};

void config(httplib::Server& server) {
  server.set_logger(baseLogger);
  server.set_error_handler(errorHandler);
}

}  // namespace littleGift
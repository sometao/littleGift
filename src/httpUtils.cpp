#include "httpUtils.h"

namespace httpUtils {

namespace contentType {

const char* html = "text/html; charset=utf-8";
const char* plain = "text/plain; charset=utf-8";
const char* json = "application/json; charset=utf-8";

}  // namespace contentType


using FrontHandler = std::function<bool(const Request&, Response&)>;
using Handler = std::function<void(const Request&, Response&)>;

Handler insertFrontAction(const FrontHandler front, const Handler action) {
  auto h = [=](const Request& req, Response& res) {
    if (!front(req, res)) {
      return;
    }
    action(req, res);
  };
  return h;
};

Handler JsonReqAction1(Handler action) {
  return [=](const Request& req, Response& res) {
    if (!req.has_header("Content-Type") ||
      req.get_header_value("Content-Type").find("application/json") == std::string::npos) {
      auto t = req.get_header_value("Content-Type");
      res.set_content("only json data supported, but got [" + t + "]", "application/json");
      return;
    }
    action(req, res);
  };
};

bool jsonCheck(const Request& req, Response& res) {
  if (!req.has_header("Content-Type") ||
    req.get_header_value("Content-Type").find("application/json") == std::string::npos) {
    auto t = req.get_header_value("Content-Type");
    res.set_content("only json data supported, but got [" + t + "]", "application/json");
    return false;
  } else {
    return true;
  }
}

Handler JsonReqAction(Handler action) { return insertFrontAction(jsonCheck, action); }

}  // namespace httpUtils

#include "littleGift.h"
#include "httpUtils.h"
#include <string>

namespace httpUtils {

namespace contentType {

const char* html = "text/html; charset=utf-8";
const char* plain = "text/plain; charset=utf-8";
const char* json = "application/json; charset=utf-8";

}  // namespace contentType


using FrontHandler = std::function<bool(const Request&, Response&)>;
using Handler = Server::Handler;

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


Handler baseAction(const std::string& actionName, const Handler action) {
  auto h = [=](const Request& req, Response& res) {
    try {
      D_LOG("access action [{}]", actionName);
      action(req, res);
      if(res.status >= 400) {
        W_LOG("[{}] action response status={}", actionName, res.status);
      }
    } catch (std::runtime_error ex) {
      res.status = 500;
      E_LOG("[{}] action runtime_error: {}", actionName, ex.what());
    } catch (...) {
      res.status = 500;
      E_LOG("[{}] unknown error.", actionName);
    }
  };
  return h;
}

Handler JsonReqAction(Handler action) { return insertFrontAction(jsonCheck, action); }

}  // namespace httpUtils

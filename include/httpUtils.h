#pragma once
#include "httplib.h"
#include <iostream>
namespace httplib {

using std::cout;
using std::endl;

class HttpUtils {
 public:
  using FrontHandler = std::function<bool(const Request&, Response&)>;
  using Handler = std::function<void(const Request&, Response&)>;

  static Handler insertFrontAction(const FrontHandler front, const Handler action) {
    auto h = [=](const Request& req, Response& res) {
      if (!front(req, res)) {
        return;
      }
      action(req, res);
    };
    return h;
  };

  static Handler JsonReqAction_bkp(Handler action) {
    return [=](const Request& req, Response& res) {
      if (!req.has_header("Content-Type") ||
          req.get_header_value("Content-Type") != "application/json") {
        auto t = req.get_header_value("Content-Type");
        res.set_content("only json data supported, but got [" + t + "]", "application/json");
        return;
      }
      action(req, res);
    };
  };

  static bool jsonCheck(const Request& req, Response& res) {
    if (!req.has_header("Content-Type") ||
      req.get_header_value("Content-Type") != "application/json") {
      auto t = req.get_header_value("Content-Type");
      res.set_content("only json data supported, but got [" + t + "]", "application/json");
      return false;
    } else {
      return true;
    }
  }

  static Handler JsonReqAction(Handler action) {
    return insertFrontAction(jsonCheck, action);
  }
};

}  // namespace httplib

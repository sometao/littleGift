#pragma once
#include "httplib.h"
#include <iostream>
namespace httpUtils {

using std::cout;
using std::endl;

namespace contentType {
  extern const char* html;
  extern const char* plain;
  extern const char* json;
}


using namespace httplib;

using FrontHandler = std::function<bool(const Request&, Response&)>;

using Handler = std::function<void(const Request&, Response&)>;

extern Handler insertFrontAction(const FrontHandler front, const Handler action);

extern Handler JsonReqAction1(Handler action);

extern bool jsonCheck(const Request& req, Response& res);

extern Handler JsonReqAction(Handler action);

}  // namespace httplib

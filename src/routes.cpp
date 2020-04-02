﻿#include "pch.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include "httpUtils.h"
#include "htmlTemplate.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

namespace littleGift {

using namespace httplib;
using Handler = Server::Handler;

namespace actions {
extern Handler helloAction;
extern Handler getParamsTest;
extern Handler formDataTest;
extern Handler helloJson;
extern Handler jsonReqTest;
}  // namespace actions

void setRoutes(httplib::Server& server) {
  int ret = -1;

  ret = server.set_mount_point("/littleGift/public", "./www");

  server.Get("/hi", [](const Request& /*req*/, Response& res) {
    res.set_content("Hello LittleGift.\n", "text/plain");
  });

  server.Get("/hello", actions::helloAction);

  server.Get("/paramsTest", actions::getParamsTest);

  server.Post("/formDataTest", actions::formDataTest);

  server.Get("/helloJson", actions::helloJson);

  server.Post("/jsonReqTest", actions::jsonReqTest);

}

}  // namespace littleGift
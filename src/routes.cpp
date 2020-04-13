#include "littleGift.h"
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
extern httplib::Logger actionLogger;
extern Handler helloAction;
extern Handler getParamsTest;
extern Handler formDataTest;
extern Handler helloJson;
extern Handler jsonReqTest;
extern Handler editor;
extern Handler result;
extern Handler saveSlides;
extern Handler root;
}  // namespace actions


void setRoutes(httplib::Server& server) {

  auto localPublic = "./resources/public";
  auto webPublic = "/littleGift/public";


  I_LOG("mount local path [{}] to web path [{}]", localPublic, webPublic);
  if (!server.set_mount_point(webPublic, localPublic)) {
    throw std::runtime_error("mount ./public failed.");
  }

  server.Get("/hi", [](const Request& /*req*/, Response& res) {
    res.set_content("Hello LittleGift.\n", "text/plain");
  });

  server.Get("/hello", actions::helloAction);

  server.Get("/paramsTest", actions::getParamsTest);

  server.Post("/formDataTest", actions::formDataTest);

  server.Get("/helloJson", actions::helloJson);

  server.Post("/jsonReqTest", actions::jsonReqTest);



  server.Get("/", actions::root);

  server.Get("/editor", actions::editor);

  server.Get("/result", actions::result);

  server.Post("/saveSlides", actions::saveSlides);
}

}  // namespace littleGift

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

//extern Handler helloAction;
//extern Handler getParamsTest;
//extern Handler formDataTest;
//extern Handler helloJson;
//extern Handler jsonReqTest;

extern Handler editor;
extern Handler result;
extern Handler saveSlides;
extern Handler root;
extern Handler getMd;
extern Handler gift;
extern Handler preview;
}  // namespace actions

extern const string baseUrl = "/littleGift";


void setRoutes(httplib::Server& server) {
  auto url = [=](const string& path) {
    string rst{baseUrl};
    if (!path.empty()) {
      rst += path;
    }
    D_LOG("create route path: [{}]", rst);
    return rst;
  };


  const string localPublic = "./resources/static";
  const string webPublic = url("/static");

  I_LOG("mount local path [{}] to web path [{}]", localPublic, webPublic);
  if (!server.set_mount_point(webPublic.c_str(), localPublic.c_str())) {
    throw std::runtime_error("mount ./public failed.");
  }

  server.Get(url("/hi").c_str(), [](const Request& /*req*/, Response& res) {
    res.set_content("Hello LittleGift.\n", "text/plain");
  });

  //server.Get(url("/hello").c_str(), actions::helloAction);
  //server.Get(url("/paramsTest").c_str(), actions::getParamsTest);
  //server.Post(url("/formDataTest").c_str(), actions::formDataTest);
  //server.Get(url("/helloJson").c_str(), actions::helloJson);
  //server.Post(url("/jsonReqTest").c_str(), actions::jsonReqTest);



  server.Get(url("/?").c_str(), actions::root);

  server.Get(url("/editor/?").c_str(), actions::editor);

  server.Get(url("/result/?").c_str(), actions::result);

  server.Post(url("/saveSlides/?").c_str(), actions::saveSlides);

  server.Get(url("/gift/?").c_str(), actions::gift);

  server.Post(url("/preview/?").c_str(), actions::preview);

  server.Get(url("/getMd/?").c_str(), actions::getMd);
}

}  // namespace littleGift

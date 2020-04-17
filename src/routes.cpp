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
//extern Handler getMd;
extern Handler giftGet;
extern Handler giftPost;
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

  server.Get(url("/gift/?").c_str(), actions::giftGet);

  server.Post(url("/gift/?").c_str(), actions::giftPost);

  server.Post(url("/preview/?").c_str(), actions::preview);

  //server.Get(url("/getMd/?").c_str(), actions::getMd);
}


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

static void config(httplib::Server& server) {
  //server.set_logger(baseLogger);
  server.set_error_handler(errorHandler);
}


static void serverListening(Server& svr, const string& host, int port) {
  I_LOG("starting server {}:{}", host, port);
  try {
    svr.listen(host.c_str(), port, 0);
  } catch (std::runtime_error ex) {
    E_LOG("startServer runtime_error: {}", ex.what());
  } catch (...) {
    E_LOG("startServer unknown error.");
  }
  W_LOG("Server thread exited.");
}



void startServer(const string& interface, int port) {
  Server svr;

  try {
    littleGift::setRoutes(svr);
    littleGift::config(svr);
    std::thread serverThread{serverListening, std::ref(svr), std::ref(interface), port};
    serverThread.detach();
    int counter = 60;
    while (!svr.is_running() && --counter > 0) {
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
  } catch (std::runtime_error ex) {
    E_LOG("server init error: {}", ex.what());
  } catch (...) {
    E_LOG("server init unknown error");
  }

  I_LOG("SERVER STARTED: {}:{}", interface, port);
  auto startTime = seeker::Time::currentTime();
  while (svr.is_running()) {
    std::this_thread::sleep_for(std::chrono::seconds(15));
    T_LOG("Server is running {} seconds", (seeker::Time::currentTime() - startTime) / 1000);
  }

  W_LOG("SERVER STOPPED.");
  cout << "DONE." << endl;
}





}  // namespace littleGift

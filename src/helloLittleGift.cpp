#include "pch.h"
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>

using std::cout;
using std::endl;
using std::string;
using std::stringstream;

namespace appUtils {
extern void encodeHTML(char* des, const char* src, int desSize);
}

namespace {
using namespace httplib;

using FrontHandler = std::function<bool(const Request&, Response&)>;
using Handler = std::function<void(const Request&, Response&)>;

const string helloPage(string name) {
  static constexpr const int maxLen = 128;

  char tmp[maxLen] = {0};
  appUtils::encodeHTML(tmp, name.c_str(), maxLen);
  cout << "tmp:\n" << tmp << endl;

  static constexpr const char fmt[] = R"(<html>
  <head>
  </head>
  <body>
    <p>Hello</p>
    <p>%s</p>
  <body>
<html>)";

  constexpr int len = sizeof(fmt);
  constexpr int size = len + maxLen;
  char out[size] = {0};

  auto r = snprintf(out, size, fmt, tmp);
  cout << "fmt:\n" << fmt << endl;
  cout << "name: " << name << endl;
  cout << "len: " << len << endl;
  cout << "size: " << size << endl;
  cout << "r: " << r << endl;
  cout << "out:\n" << out << endl;
  return out;
}

auto helloAction = [](const Request& req, Response& res) {
  cout << "++++++ hello 2 ++++++" << endl;
  if (req.has_param("name")) {
    auto name = req.get_param_value("name");
    res.set_content(helloPage(name + "_2"), "text/html");
  } else {
    res.set_content("error: No param [name] found.\n", "text/plain");
  }
};

auto getParamsTest = [](const Request& req, Response& res) {
  cout << "++++++ getParamsTest ++++++" << endl;
  if (req.has_param("p1") && req.has_param("p2")) {
    auto v1 = req.get_param_value("p1");
    auto v2 = req.get_param_value("p2");
    stringstream result;
    result << "p1=" << v1 << "\np2=" << v2;
    res.set_content(result.str(), "text/html");
  } else {
    res.status = 404;
  }
};

// TODO form request;

auto formDataTest = [](const Request& req, Response& res) {
  cout << "++++++ formDataTest ++++++" << endl;

  auto size = req.files.size();

  // file.filename;
  // file.content_type;
  // file.content;

  if (req.has_file("fp1") && req.has_file("fp2")) {
    const auto& file1 = req.get_file_value("fp1");
    const auto& file2 = req.get_file_value("fp2");
    auto fv1 = file1.content;
    auto fv2 = file2.content;
    stringstream result;
    result << "fp1=" << fv1 << "\nfp2=" << fv2;
    cout << "result:" << result.str() << endl;
    res.set_content(result.str(), "text/html");
  } else {
    res.status = 404;
  }
};

auto helloJson = [](const Request& req, Response& res) {
  stringstream result;
  result << R"({"title": "hello Json", "age": 100})";
  res.set_content(result.str(), "application/json");
};

FrontHandler jsonCheck = [](const Request& req, Response& res) {
  if (!req.has_header("Content-Type") ||
      req.get_header_value("Content-Type") != "application/json") {
    auto t = req.get_header_value("Content-Type");
    res.set_content("only json data supported, but got [" + t + "]", "application/json");
    return false;
  } else {
    return true;
  }
};


Handler insertFrontAction(FrontHandler front, Handler action) {
  auto h = [=](const Request& req, Response& res) {
    if (!jsonCheck(req, res)) {
      return;
    }
    action(req, res);
  };
  return h;
}


Handler JsonReqAction(Server::Handler action) {
  return [=](const Request& req, Response& res) {
    if (!req.has_header("Content-Type") ||
        req.get_header_value("Content-Type") != "application/json") {
      auto t = req.get_header_value("Content-Type");
      res.set_content("only json data supported, but got [" + t + "]", "application/json");
      return;
    }
    action(req, res);
  };
}

auto jsonReqTest1 = JsonReqAction([](const Request& req, Response& res) {
  if (req.has_header("Content-Length")) {
    auto val = req.get_header_value("Content-Length");
    cout << "Content-Length:" << val << endl;
  }

  auto b = req.body;
  auto body = b.c_str();
  spdlog::info("req body: {}", body);

  stringstream result;
  result << R"({"title": "JsonReqTest", "age": 100})";
  spdlog::info("result: {}", result.str());

  res.set_content(result.str(), "application/json");
});

void jsonReqTest2(const Request& req, Response& res) {

  auto b = req.body;
  auto body = b.c_str();
  spdlog::info("jsonReqTest!!!: {}", body);

  stringstream result;
  result << R"({"title": "jsonReqTest!!", "age": 102})";
  spdlog::info("result: {}", result.str());

  res.set_content(result.str(), "application/json");

};


auto jsonReqTest3 = insertFrontAction(jsonCheck, jsonReqTest2);


}  // namespace

void setRoutes(httplib::Server& server) {
  int ret = -1;
  ret = server.set_mount_point("/public", "./www");

  server.Get("/hi", [](const Request& /*req*/, Response& res) {
    res.set_content("Hello LittleGift.\n", "text/plain");
  });

  server.Get("/hello", helloAction);

  server.Get("/paramsTest", getParamsTest);

  server.Post("/formDataTest", formDataTest);

  server.Get("/helloJson", helloJson);

  // TODO handle json request, need some json parser.
  server.Post("/jsonReqTest1", jsonReqTest1);

  server.Post("/jsonReqTest2", jsonReqTest2);


  server.Post("/jsonReqTest3", jsonReqTest3);

  // TODO need a log tool.
}

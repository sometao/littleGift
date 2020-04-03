#include "pch.h"
#include <iostream>
#include <sstream>
#include <string>
#include "httpUtils.h"
#include "htmlTemplate.h"

namespace littleGift {
  using std::cout;
  using std::endl;
  using std::string;
  using std::stringstream;


  namespace pages {

    extern const string helloPage(string name);
  }

  namespace actions {

    using namespace httplib;

    using Handler = Server::Handler;

    Handler helloAction = [](const Request& req, Response& res) {
      cout << "++++++ hello 2 ++++++" << endl;
      if (req.has_param("name")) {
        auto name = req.get_param_value("name");
        res.set_content(pages::helloPage(name + "_2"), "text/html");
      } else {
        res.set_content("error: No param [name] found.\n", "text/plain");
      }
    };

    Handler getParamsTest = [](const Request& req, Response& res) {
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

    Handler formDataTest = [](const Request& req, Response& res) {
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

    Handler helloJson = [](const Request& req, Response& res) {
      stringstream result;
      result << R"({"title": "hello Json", "age": 100})";
      res.set_content(result.str(), "application/json");
    };

    Handler jsonReqTest = HttpUtils::JsonReqAction([](const Request& req, Response& res) {
      cout << "jsonReqTest  ----  1" << endl;

      if (req.has_header("Content-Length")) {
        cout << "jsonReqTest  ----  2" << endl;
        auto val = req.get_header_value("Content-Length");
        cout << "Content-Length:" << val << endl;
      }
      cout << "jsonReqTest  ----  3" << endl;

      auto b = req.body;
      cout << "jsonReqTest  ----  4" << endl;
      auto body = b.c_str();
      cout << "jsonReqTest  ----  5" << endl;
      spdlog::info("req body: {}", body);

      stringstream result;
      result << R"({"title": "JsonReqTest", "age": 100})";
      spdlog::info("result: {}", result.str());

      res.set_content(result.str(), "application/json");
      });

  }  // namespace actions

}  // namespace littleGift
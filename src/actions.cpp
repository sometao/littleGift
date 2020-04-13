#include "littleGift.h"
#include <iostream>
#include <sstream>
#include <string>
#include "httpUtils.h"
#include "htmlTemplate.h"
#include "seeker/secure.h"
#include "seeker/common.h"
#include "dao.h"
#include <regex>


namespace littleGift {
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

namespace pages {

extern const string editor();
extern const string result(std::shared_ptr<dao::SlidesRow> row);
extern const string gift(const string& mdUri);

}  // namespace pages



namespace actions {

using namespace httplib;
using Handler = Server::Handler;
using httpUtils::baseAction;

Handler root =
    baseAction("root", [](const Request& req, Response& res) { res.set_redirect("/editor"); });


Handler editor = baseAction("editor", [](const Request& req, Response& res) {
  res.set_content(pages::editor(), httpUtils::contentType::html);
});


Handler saveSlides = baseAction("saveSlides", [](const Request& req, Response& res) {
  static auto reg = std::regex(R"(\r\n?)");

  if (req.has_file("mdContent")) {
    const auto& value = req.get_file_value("mdContent");
    auto mdContent = std::regex_replace(value.content, reg, "\n");
    string author = req.has_file("author") ? req.get_file_value("author").content : "";
    auto timestamp = seeker::Time::currentTime();
    string token = seeker::Secure::md5(mdContent + std::to_string(timestamp));
    token = seeker::String::toLower(token);
    string code = seeker::Secure::randomChars(6);

    dao::SlidesRow row{-1, author, mdContent, "", token, code, timestamp};
    auto id = dao::addSlides(row);

    I_LOG("Slide added: mdContent={}, author={}, token={}, code={}, timestamp={}",
          mdContent.substr(0, 16) + "..",
          author,
          token,
          code,
          timestamp);
    res.status = 303;
    res.headers.insert({"Location", "/result?token=" + token});
  } else {
    W_LOG("can not fine [mdContent] error in request form.");
    res.status = 200;
    res.set_content("markdown content can not be empty.", httpUtils::contentType::plain);
  }
});


Handler result = baseAction("result", [](const Request& req, Response& res) {
  if (req.has_param("token")) {
    auto token = req.get_param_value("token");
    I_LOG("get result for token={}", token);
    auto row = dao::getSlides(token);
    res.set_content(pages::result(row), httpUtils::contentType::html);
  } else {
    res.set_redirect("/");
  }
});

Handler getMd = baseAction("getMd", [](const Request& req, Response& res) {
  auto emptyLine = std::regex{R"(^[ \t]+$)"};
  auto fourEmptyLine = std::regex{R"(\n{5,})"};
  if (req.has_param("token")) {
    auto token = req.get_param_value("token");
    I_LOG("getMd for token={}", token);
    auto row = dao::getSlides(token);
    string content = row->content;
    content = std::regex_replace(content, emptyLine, "");
    content = std::regex_replace(content, fourEmptyLine, "\n\n\n\n");
    content = seeker::String::removeLastEmptyLines(content);
    res.set_content(content, httpUtils::contentType::plain);
  } else {
    res.set_content("Can not fine your resources.", httpUtils::contentType::plain);
  }
});


Handler gift = baseAction("gift", [](const Request& req, Response& res) {
  if (req.has_param("token")) {
    auto token = req.get_param_value("token");
    I_LOG("get result for token={}", token);
    string mdUri = "/getMd?token=" + token;
    res.set_content(pages::gift(mdUri), httpUtils::contentType::html);
  } else {
    res.set_redirect("/");
  }
});



}  // namespace actions

}  // namespace littleGift
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
extern const string preview(const string& mdContent);
extern const string codeChecker(const string& mdContent);

}  // namespace pages

extern const string baseUrl;

namespace actions {

using namespace httplib;
using Handler = Server::Handler;
using httpUtils::baseAction;

static string formatMd(const string& mdContent) {


  static std::regex reg = std::regex(R"(\r\n?)");
  static std::regex emptyLine = std::regex{R"(^[ \t]+$)"};
  static std::regex fourEmptyLine = std::regex{R"(\n{5,})"};

  string rst{mdContent};

  rst = std::regex_replace(rst, reg, "\n");
  rst = std::regex_replace(rst, emptyLine, "");
  rst = std::regex_replace(rst, fourEmptyLine, "\n\n\n\n");
  rst = seeker::String::removeLastEmptyLines(rst);
  return rst;
}

Handler root = baseAction("root", [](const Request& req, Response& res) {
  string path = baseUrl + "/editor";
  W_LOG("root: {}", path);
  res.set_redirect(path.c_str());
});


Handler editor = baseAction("editor", [](const Request& req, Response& res) {
  res.set_content(pages::editor(), httpUtils::contentType::html);
});


Handler saveSlides = baseAction("saveSlides", [](const Request& req, Response& res) {

  if (req.has_file("mdContent")) {
    const auto& value = req.get_file_value("mdContent");
    auto mdContent = value.content;
    string author = req.has_file("author") ? req.get_file_value("author").content : "";
    string code = req.has_file("code") ? req.get_file_value("code").content : "";

    string token = seeker::Secure::sha1(mdContent + seeker::Secure::randomChars(16));
    token = seeker::String::toLower(token);

    auto timestamp = seeker::Time::currentTime();

    dao::SlidesRow row{-1, author, mdContent, "", token, code, timestamp};
    auto id = dao::addSlides(row);

    I_LOG("Slide added: mdContent={}, author={}, token={}, code={}, timestamp={}",
          mdContent.substr(0, 16) + "..",
          author,
          token,
          code,
          timestamp);

    //TODO return result page instead of redirect.
    res.status = 303;
    res.headers.insert({"Location", baseUrl + "/result?token=" + token});
  } else {
    W_LOG("can not fine [mdContent] error in request form.");
    res.status = 200;
    res.set_content("markdown content can not be empty.", httpUtils::contentType::plain);
  }
});

//TODO no need it any more?
Handler result = baseAction("result", [](const Request& req, Response& res) {
  if (req.has_param("token")) {
    auto token = req.get_param_value("token");
    I_LOG("get result for token={}", token);
    auto row = dao::getSlides(token);
    res.set_content(pages::result(row), httpUtils::contentType::html);
  } else {
    res.set_redirect(baseUrl.c_str());
  }
});


//FIXME not need any more.
//Handler getMd = baseAction("getMd", [](const Request& req, Response& res) {
//
//  if (req.has_param("token")) {
//    auto token = req.get_param_value("token");
//    I_LOG("getMd for token={}", token);
//    auto row = dao::getSlides(token);
//    string content = formatMd(row->content);
//    res.set_content(content, httpUtils::contentType::plain);
//  } else {
//    res.set_content("Can not fine your resources.", httpUtils::contentType::plain);
//  }
//});


Handler giftGet = baseAction("giftGet", [](const Request& req, Response& res) {
  if (req.has_param("token")) {
    auto token = req.get_param_value("token");
    auto row = dao::getSlides(token);
    if(row->editCode.empty()) {
      string mdContent = formatMd(row->content);
      res.set_content(pages::preview(mdContent), httpUtils::contentType::html);
    } else {
      res.set_content(pages::codeChecker(token), httpUtils::contentType::html);
    }
  } else {
    W_LOG("giftGet can not get gift without token.");
    res.status = 404;
  }
});

Handler giftPost = baseAction("giftPost", [](const Request& req, Response& res) {
  if (req.has_file("token") && req.has_file("code") ) {
    const auto& value = req.get_file_value("token");
    auto token = req.get_file_value("token").content;
    auto code = req.get_file_value("code").content;
    I_LOG("giftPost for token={} with code={}", token, code);
    auto row = dao::getSlides(token);
    if(seeker::String::trim(code) == row->editCode) {
      string mdContent = formatMd(row->content);
      res.set_content(pages::preview(mdContent), httpUtils::contentType::html);
    } else {
      res.set_content(pages::codeChecker(token), httpUtils::contentType::html);
    }
  } else {
    W_LOG("giftPost form data error.");
    res.status = 404;
  }
});


Handler preview = baseAction("preview", [](const Request& req, Response& res) {
  if (req.has_file("mdContent")) {
    const auto& value = req.get_file_value("mdContent");
    string mdContent = formatMd(value.content);
    res.set_content(pages::preview(mdContent), httpUtils::contentType::html);
  } else {
    W_LOG("Can not preview gift without mdContent.");
    res.set_redirect(baseUrl.c_str());
  }
});



}  // namespace actions

}  // namespace littleGift
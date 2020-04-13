#include "littleGift.h"
#include <iostream>
#include <sstream>
#include <string>
#include "httpUtils.h"
#include "htmlTemplate.h"
#include "seeker/secure.h"
#include "seeker/common.h"
#include "dao.h"

namespace littleGift {
using std::cout;
using std::endl;
using std::string;
using std::stringstream;

namespace pages {

extern const string editor();
extern const string result(std::shared_ptr<dao::SlidesRow> row);

}  // namespace pages



namespace actions {

using namespace httplib;
using Handler = Server::Handler;
using httpUtils::baseAction;

Handler root = baseAction("root", [](const Request& req, Response& res) {
  res.set_redirect("/editor");
});


Handler editor = baseAction("editor", [](const Request& req, Response& res) {
  res.set_content(pages::editor(), httpUtils::contentType::html);
});

// TODO to be test
Handler saveSlides = baseAction("saveSlides", [](const Request& req, Response& res) {
  //D_LOG("files size = {}", req.files.size());
  //D_LOG("body = {}", req.body);

  //auto it = req.headers.find("Content-Type");
  //if (it != req.headers.end()) {
  //  D_LOG("Content-Type = {}: {}", it->first, it->second);
  //}

  //auto files = req.files;
  //for (auto& f : files) {
  //  D_LOG("saveSlides file:  {} = {}, {}", f.first, f.second.filename, f.second.name);
  //}

  if (req.has_file("mdContent")) {
    const auto& value = req.get_file_value("mdContent");

    auto mdContent = value.content;
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

Handler result = baseAction("result",[](const Request& req, Response& res) {

  if (req.has_param("token")) {
    auto token = req.get_param_value("token");
    I_LOG("get result for token={}", token);
    auto row = dao::getSlides(token);
    res.set_content(pages::result(row), httpUtils::contentType::html);
  } else {
    res.set_redirect("/");
  }

});





}  // namespace actions

}  // namespace littleGift
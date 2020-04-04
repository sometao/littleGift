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

extern const string editor();
extern const string result(const string& name, const string& code );

}

namespace actions {

using namespace httplib;

using Handler = Server::Handler;



Handler editor= [](const Request& req, Response& res) {
  res.set_content(pages::editor(), "text/html");
};


Handler saveSlices = [](const Request& req, Response& res) {
  if (req.has_file("mdContent")) {
    const auto& value = req.get_file_value("mdContent");
    auto mdContent = value.content;

    spdlog::trace("");

    stringstream result;
    result << "got you slices." << endl;
    result << "mdContent:\n" << mdContent;
    res.set_content(result.str(), "text/plain");
  } else {
    res.status = 404;
  }

  res.set_content(pages::editor(), "text/html");
};



Handler result = [](const Request& req, Response& res) {
  res.set_content(pages::result("Tom", "#lala\n- aaa\n- bbb"), "text/html");
};


}  // namespace actions

}  // namespace littleGift
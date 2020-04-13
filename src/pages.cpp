#include "littleGift.h"
#include "htmlTemplate.h"
#include "dao.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>

namespace littleGift {
extern void encodeHTML(char* des, const char* src, int desSize);

namespace pages {

using std::cout;
using std::endl;
using std::string;


const string editor() {
  static auto htmlPattern = httpTemplate::Engine::loadTemplate("editorPage.html");
  static const std::vector<string> emptyArgs{};
  auto outHtml = htmlPattern->genHtml(emptyArgs);
  return outHtml;
}

const string result( std::shared_ptr<dao::SlidesRow> row) {
  static auto htmlPattern = httpTemplate::Engine::loadTemplate("result.html");
  std::vector<string> args{row->content, row->accessToken, row->editCode, row->authorName, std::to_string(row->createTime)};
  auto outHtml = htmlPattern->genHtml(args);
  return outHtml;
}


const string gift(const string& mdUri) {
  static auto htmlPattern = httpTemplate::Engine::loadTemplate("gift.html");
  std::vector<string> args{mdUri};
  auto outHtml = htmlPattern->genHtml(args);
  return outHtml;
}






const string helloPage(string name) {
  static constexpr const int maxLen = 128;

  char tmp[maxLen] = {0};
  encodeHTML(tmp, name.c_str(), maxLen);
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

}  // namespace pages

}  // namespace littleGift

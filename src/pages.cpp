#include "htmlTemplate.h"
#include <iostream>
#include <string>

namespace littleGift {
extern void encodeHTML(char* des, const char* src, int desSize);

namespace pages {

using std::cout;
using std::endl;
using std::string;

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

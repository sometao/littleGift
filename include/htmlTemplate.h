#pragma once

#include <iostream>
#include <unordered_map>
#include <functional>
#include <vector>
#include <fstream>
#include <string>
#include <regex>

namespace hTemplate {

using std::cout;
using std::endl;
using std::ifstream;
using std::regex;
using std::string;
using std::unordered_map;
using std::vector;

class HtmlTemplate {
  string name;
  std::function<string(const vector<string>&)> func;

 public:
  string genHtml(const vector<string>& args) { return func(args); }
};

class StringUtils {
 public:
  static std::vector<string> split(const string& target, const string& sp) {
    std::vector<string> rst{};
    const auto spLen = sp.length();
    string::size_type pos = 0;
    auto f = target.find(sp, pos);
    while (f != string::npos) {
      auto r = target.substr(pos, f - pos);
      rst.emplace_back(r);
      pos = f + spLen;
      f = target.find(sp, pos);
    }
    return rst;
  }

  static string removeBlank(const string& target) {
    static std::regex blankRe{R"(\s+)"};
    return std::regex_replace(target, blankRe, "");
  }
};

class Engine {
  Engine(){};
  unordered_map<string, HtmlTemplate> templateMap;
  static Engine& getInstance() {
    static Engine instance;
    return instance;
  }

  void loadTemplateImp(const string& templateFile) {
    static const regex argsLineRe{R"(Args\((.*)\))"};


    std::smatch sm;

    auto getArgs = [&](string ln) -> bool {
      if (std::regex_search(ln, sm, argsLineRe)) {
        auto argsStr = sm.str();
        argsStr = StringUtils::removeBlank(argsStr);
        auto args = StringUtils::split(argsStr, ",");
        return true;
      } else {
        return false;
      }
    };

    string htmlString{};

    {
      string line{};
      bool argsGot = false;
      ifstream is{templateFile};
      while (std::getline(is, line)) {
        if (!argsGot) {
          argsGot = getArgs(line);
          continue;
        }
        htmlString += line;
        htmlString += "\n";
      }
    }

    //TODO replace args.
    






  }

 public:
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  ~Engine() { cout << "SingleClass destructed." << endl; }

  static void loadTemplate(const string& templateFile) {
    auto& ins = getInstance();
    ins.loadTemplateImp(templateFile);
  };
};

}  // namespace hTemplate

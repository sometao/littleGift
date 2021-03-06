#pragma once

#include <iostream>
#include <unordered_map>
#include <functional>
#include <vector>
#include <fstream>
#include <string>
#include <regex>
#include <memory>
#include <iterator>
#include "seeker/common.h"

#ifndef HTML_TEMPLATE_DIR
#define HTML_TEMPLATE_DIR "./resources/htmlTemplate/"
#endif  // !HTML_TEMPLATE_DIR

namespace httpTemplate {

using std::cout;
using std::endl;
using std::string;
using std::vector;


class HtmlTemplate {
  static std::regex getRegex(string argName) {
    // <!--@title[-->一些文字内容<!--]title@-->
    // string regStr = "@\\{\\{" + argName + "\\}\\}.*@";
    // string regStr = "<!--@" + argName + "-->.*<!--" + argName + "@-->";
    // string regStr = "<!--@" + argName + "-->(\n|.)*<!--" + argName + "@-->";
    string regStr = "<!--@" + argName + "\\[-->.*<!--\\]" + argName + "@-->";
    return std::regex{regStr, std::regex::optimize};
  };

  static vector<std::regex> patternInitHalper(vector<string>& argNameList) {
    vector<std::regex> ls{};
    std::transform(argNameList.begin(), argNameList.end(), std::back_inserter(ls),
                   [](const string& argName) { return getRegex(argName); });
    return ls;
  }

 public:
  const string templateName;
  const string htmlString;
  const vector<string> argsNameList;
  const vector<std::regex> argsPatternList{};

  HtmlTemplate(const string& templateName_, const string& template_,
               vector<string>& argsNameList_)
      : templateName(templateName_),
        htmlString(template_),
        argsNameList(argsNameList_),
        argsPatternList(patternInitHalper(argsNameList_)) {
    // cout << "HtmlTemplate create: " << templateName << endl;
  }

  string genHtml(const vector<string>& args) {
    if (args.size() != argsPatternList.size()) {
      throw std::runtime_error("args error for template[" + templateName + "]");
    } else {
      string outHtml = htmlString;
      for (size_t i = 0; i < argsPatternList.size(); i++) {
        auto p = argsPatternList.at(i);
        auto value = args.at(i);
        outHtml = std::regex_replace(outHtml, p, value);
      }
      return outHtml;
    }
  }
};

class Engine {
  Engine(){};

  const string TEMPLATE_DIR = HTML_TEMPLATE_DIR;

  std::unordered_map<string, std::shared_ptr<HtmlTemplate>> templateMap{};
  static Engine& getInstance() {
    static Engine instance{};
    return instance;
  }

  std::shared_ptr<HtmlTemplate> getTemplateImp(const string& templateName) {
    auto r = templateMap.find(templateName);
    if (r != templateMap.end()) {
      return r->second;
    } else {
      return std::shared_ptr<HtmlTemplate>{};
    }
  };

  std::shared_ptr<HtmlTemplate> loadTemplateImp(const string& templateName) {
    //<!--Args(title)-->

    static const std::regex argsLineRe{R"(<!--Args\((.*)\)-->)"};
    static const std::regex winPathSplitorRe{R"(\\)"};

    std::smatch sm;
    std::vector<string> args;

    auto getArgs = [&](string ln) -> bool {
      if (std::regex_search(ln, sm, argsLineRe)) {
        auto argsStr = sm.str(1);
        // cout << "argsStr:" << argsStr << endl;
        argsStr = seeker::String::removeBlanks(argsStr);
        // cout << "argsStr processed:" << argsStr << endl;
        args = seeker::String::split(argsStr, ",");
        return true;
      } else {
        return false;
      }
    };

    string htmlString{};
    string templateFilePath = TEMPLATE_DIR + templateName;
    {
      bool argsGot = false;
      std::ifstream is{templateFilePath};
      if (is.is_open()) {
        string line{};
        while (std::getline(is, line)) {
          if (!argsGot) {
            argsGot = getArgs(line);
            continue;
          }
          htmlString += line;
          htmlString += "\n";
        }
      } else {
        throw std::runtime_error("can not open template file: " + templateFilePath);
      }
    }

    auto temp = std::make_shared<HtmlTemplate>(templateName, htmlString, args);
    templateMap.insert_or_assign(templateName, temp);
    return templateMap.at(templateName);
  }

 public:
  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  ~Engine() { cout << "SingleClass destructed." << endl; }

  static std::shared_ptr<HtmlTemplate> loadTemplate(const string& templateFile) {
    auto& ins = getInstance();
    return ins.loadTemplateImp(templateFile);
  };

  static std::shared_ptr<HtmlTemplate> getTemplate(const string& templateName) {
    auto& ins = getInstance();
    return ins.getTemplateImp(templateName);
  }
};

}  // namespace httpTemplate

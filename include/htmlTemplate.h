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

namespace hTemplate {

using std::cout;
using std::endl;
using std::string;
using std::vector;

class StringUtils {
 public:
  static std::vector<string> split(const string& target, const string& sp) {
    std::vector<string> rst{};
    const auto spLen = sp.length();
    string::size_type pos = 0;
    auto f = target.find(sp, pos);
    if (f == string::npos) {
      rst.emplace_back(target);
    } else {
      while (f != string::npos) {
        auto r = target.substr(pos, f - pos);
        rst.emplace_back(r);
        pos = f + spLen;
        f = target.find(sp, pos);
      }
    }
    return rst;
  }

  static string removeBlanks(const string& target) {
    static std::regex blankRe{R"(\s+)"};
    return std::regex_replace(target, blankRe, "");
  }
};

class HtmlTemplate {
  static vector<std::regex> patternInitHalper(vector<string>& argNameList) {
    vector<std::regex> ls{};
    std::transform(argNameList.begin(), argNameList.end(), std::back_inserter(ls),
                   [](const string& argName) -> std::regex {
                     return std::regex{"@\\{\\{" + argName + "\\}\\}@"};
                   });
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
    cout << "HtmlTemplate create: " << templateName << endl;
  }

  string genHtml(const vector<string>& args) {
    // TODO implement.
    return "";
  }
};

class Engine {
  Engine(){};
  std::unordered_map<string, std::shared_ptr<HtmlTemplate>> templateMap{};
  static Engine& getInstance() {
    static Engine instance;
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

  std::shared_ptr<HtmlTemplate> loadTemplateImp(const string& templateFile) {
    //<!--Args(title)-->

    static const std::regex argsLineRe{R"(<!--Args\((.*)\)-->)"};
    static const std::regex winPathSplitorRe{R"(\\)"};

    std::smatch sm;
    std::vector<string> args;

    auto getArgs = [&](string ln) -> bool {
      if (std::regex_search(ln, sm, argsLineRe)) {
        auto argsStr = sm.str(1);
        cout << "find args line:" << argsStr << endl;
        argsStr = StringUtils::removeBlanks(argsStr);
        cout << "find args line process:" << argsStr << endl;
        args = StringUtils::split(argsStr, ",");
        for (auto& a : args) {
          cout << "arg:\n" << a << endl;
        }
        return true;
      } else {
        return false;
      }
    };

    string htmlString{};
    string templateName;

    {
      bool argsGot = false;
      std::ifstream is{templateFile};
      if (is.is_open()) {
        templateName = std::regex_replace(templateFile, winPathSplitorRe, "/");
        auto namePos = templateName.find_last_of("/");
        if (namePos != string::npos) {
          namePos += 1;
          templateName = templateName.substr(namePos, templateName.length() - namePos);
        }

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
        throw std::runtime_error("can not open template file: " + templateFile);
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

  static std::shared_ptr<HtmlTemplate> getTemplate(const string& templateName) {}
};

}  // namespace hTemplate

#pragma once

#include <iostream>
#include <unordered_map>
#include <functional>
#include <vector>
#include <fstream>
#include <string>

namespace hTemplate {

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;
using std::ifstream;

class HtmlTemplate {
  string name;
  std::function<string(const vector<string>&)> func;

 public:
  string genHtml(const vector<string>& args) { return func(args); }
};

class Engine {
  Engine(){};
  unordered_map<string, HtmlTemplate> templateMap;
  static Engine& getInstance() {
    static Engine instance;
    return instance;
  }

  void innerLoadTemplate(const string& templateFile) {
    ifstream is{templateFile};
    string line;
    while (std::getline(is, line)) {

    }

  }

 public:

  Engine(const Engine&) = delete;
  Engine& operator=(const Engine&) = delete;
  ~Engine() { cout << "SingleClass destructed." << endl; }

  static void loadTemplate(const string& templateFile){
    auto& ins = getInstance();
    ins.innerLoadTemplate(templateFile);
  };


};



}  // namespace hTemplate

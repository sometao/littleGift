#include "pch.h"
#include "htmlTemplate.h"
#include <string>
#include "sqlite3.h"

using std::cout;
using std::endl;
using std::string;

namespace littleGift {
namespace test {




void testHttpClient() {
  using namespace httplib;

#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  // httplib::SSLClient cli("localhost", 8080);
  httplib::SSLClient cli("www.baidu.com");
  // httplib::SSLClient cli("google.com");
  // httplib::SSLClient cli("www.youtube.com");
  cli.set_ca_cert_path(CA_CERT_FILE);
  cli.enable_server_certificate_verification(true);
#else
  httplib::Client cli("localhost", 8080);
#endif
  cout << "GET request: www.baidu.com" << endl;
  auto res = cli.Get("/");

  cout << "resutlt get." << endl;

  if (res) {
    cout << res->status << endl;
    cout << res->get_header_value("Content-Type") << endl;
    cout << res->body << endl;
  } else {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
    auto result = cli.get_openssl_verify_result();
    if (result) {
      cout << "verify error: " << X509_verify_cert_error_string(result) << endl;
    }
    cout << result << endl;
#endif
  }
}

void setLogger() {
  spdlog::info("Welcome to spdlog!");
  spdlog::error("Some error message with arg: {}", 1);

  spdlog::warn("Easy padding in numbers like {:08d}", 12);
  spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  spdlog::info("Support for floats {:03.2f}", 1.23456);
  spdlog::info("Positional args are {1} {0}..", "too", "supported");
  spdlog::info("{:<30}", "left aligned");

  spdlog::set_level(spdlog::level::debug);  // Set global log level to debug
  spdlog::debug("This message should be displayed..");

  // change log pattern
  spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

  // Compile time log levels
  // define SPDLOG_ACTIVE_LEVEL to desired level
  SPDLOG_TRACE("Some trace message with param {}", 42);
  SPDLOG_DEBUG("Some debug message");

  // Set the default logger to file logger
  // auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
  // auto file_logger = spdlog::basic_logger_mt("async_factory", "logs/basic.txt");
  // spdlog::set_default_logger(file_logger);
  // spdlog::flush_every(std::chrono::seconds(1));
}

void testHtmlTemplate() {
  using httpTemplate::Engine;
  using httpTemplate::HtmlTemplate;

  // string targetFile = R"(D:\workstation\vs\littleGift\htmlTemplate\test2.html)";
  // string targetFile = R"(.\htmlTemplate\test2.html)";
  // string targetFile = "hello.html";
  string targetFile = "editorPage.html";

  auto t = Engine::loadTemplate(targetFile);

  if (t != nullptr) {
    cout << "name:" << t->templateName << endl;
    cout << " ------------ " << endl;
    for (auto& name : t->argsNameList) {
      cout << "arg name: " << name << endl;
    }
    cout << " ------------ " << endl;
    cout << "html:\n" << t->htmlString << endl;

    std::vector<string> args{};
    // std::vector<string> args{ "hello, html template.", "2020哈哈" , "我得名字" };

    auto out = t->genHtml(args);
    cout << "----------------  out html -------------------:\n" << out << endl;

    string outFile = "outHtml1.html";
    std::ofstream os{outFile};
    os << out;
    cout << "write html to:" << outFile << endl;

  } else {
    cout << "cant find template:" << targetFile << endl;
  }
}

void testStringSplit() {
  // string target = "title,date";
  string target = "";

  auto rst = seeker::String::split(target, ",");

  cout << "size:" << rst.size() << endl;
  for (auto& a : rst) {
    cout << "a: " << a << endl;
  }
}

void testChinese() {
  cout << "I am English." << endl;
  cout << "我是中文" << endl;
  cout << "I am 中国人" << endl;
}

int main999() {
  // system("chcp 65001");
  // auto j = httplib::jsonCheck;
  // cout << "j: " << (j == nullptr) << endl;

  // httpTemplate::Engine::loadTemplate("test2.html");
  // testChinese();
  // testStringSplit();
  testHtmlTemplate();

  return 0;
}


}  // namespace test


}  // namespace littleGift

#include "pch.h"
#include "httplib.h"
#include <iostream>
#include <string>
#include <cstring>

using std::cout;
using std::endl;
using std::string;

extern void setRoutes(httplib::Server& server);

void setLogger() {

  spdlog::info("Welcome to spdlog!");
  spdlog::error("Some error message with arg: {}", 1);

  spdlog::warn("Easy padding in numbers like {:08d}", 12);
  spdlog::critical("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
  spdlog::info("Support for floats {:03.2f}", 1.23456);
  spdlog::info("Positional args are {1} {0}..", "too", "supported");
  spdlog::info("{:<30}", "left aligned");

  spdlog::set_level(spdlog::level::debug); // Set global log level to debug
  spdlog::debug("This message should be displayed..");

  // change log pattern
  spdlog::set_pattern("[%H:%M:%S %z] [%n] [%^---%L---%$] [thread %t] %v");

  // Compile time log levels
  // define SPDLOG_ACTIVE_LEVEL to desired level
  SPDLOG_TRACE("Some trace message with param {}", 42);
  SPDLOG_DEBUG("Some debug message");

  // Set the default logger to file logger
  //auto file_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
  //auto file_logger = spdlog::basic_logger_mt("async_factory", "logs/basic.txt");
  //spdlog::set_default_logger(file_logger);
  //spdlog::flush_every(std::chrono::seconds(1));


}

int main() {
#ifdef CPPHTTPLIB_ZLIB_SUPPORT
  cout << "CPPHTTPLIB_ZLIB_SUPPORT defined." << endl;
#else
  cout << "CPPHTTPLIB_ZLIB_SUPPORT not defined." << endl;
#endif 
  cout << "DONE." << endl;


  return 0;
}

int main0() {
  using namespace httplib;

  setLogger();

  cout << " ---  " << endl;
  spdlog::info("hello, little Gift.");
  spdlog::info("hello, little Gift.");
  spdlog::info("hello, little Gift.");
  spdlog::info("hello, little Gift.");
  spdlog::info("hello, little Gift.");
  spdlog::info("hello, little Gift.");

  cout << " ---  " << endl;

  Server svr;

  svr.set_error_handler([](const auto& req, auto& res) {
    auto fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");
  });


  setRoutes(svr);


  svr.listen("localhost", 50080);

  cout << "DONE." << endl;

  return 0;
}

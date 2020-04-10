#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async.h"

#include <iostream>
#include <memory>
#include <string>



// short for logger, you may copy following define to your CPP/H file.
#define T_LOG(...) SPDLOG_TRACE(__VA_ARGS__)
#define D_LOG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define I_LOG(...) SPDLOG_INFO(__VA_ARGS__)
#define W_LOG(...) SPDLOG_WARN(__VA_ARGS__)
#define E_LOG(...) SPDLOG_ERROR(__VA_ARGS__)

namespace seeker {

  using std::string;

// TODO test logger.
class Logger {
  Logger(const string& logFile, bool stdOutOn, bool fileOutOn, const string& pattern, bool useAsyn) {

    const string defaultLogFile = "./application.log";
    auto logFileName = logFile.length() > 0 ? logFile : defaultLogFile;

    const string defaultPattern = "[%Y%m%d %H:%M:%S.%e %s:%#] [%^%L%$]: %v";
    const string usePattern = pattern.length() > 0 ? pattern : defaultPattern;

    std::cout << "init Logger with pattern: " << usePattern << std::endl;
    std::cout << "Logger stdOutOn=[" << stdOutOn << "] fileOutOn=[" << fileOutOn << "]"
              << std::endl;
    std::cout << "Logger logFileName=[" << logFileName << "]" << std::endl;

    try {
      auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      console_sink->set_level(spdlog::level::trace);
      console_sink->set_pattern(usePattern);

      auto file_sink =
          std::make_shared<spdlog::sinks::daily_file_sink_mt>(logFileName, 23, 59);
      file_sink->set_level(spdlog::level::trace);
      file_sink->set_pattern(usePattern);

      std::vector<spdlog::sink_ptr> sinks;
      if (stdOutOn) {
        sinks.push_back(console_sink);
      }
      if (fileOutOn) {
        sinks.push_back(file_sink);
      }

      std::shared_ptr<spdlog::logger> logger;
      if (useAsyn) {
        logger = getAsyncLogger(sinks);
      } else {
        logger = getLogger(sinks);
      }

      spdlog::set_default_logger(logger);

      spdlog::set_level(spdlog::level::trace);
      spdlog::flush_on(spdlog::level::warn);
      spdlog::flush_every(std::chrono::seconds(2));

    } catch (const spdlog::spdlog_ex& ex) {
      std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
  };

  std::shared_ptr<spdlog::async_logger> getAsyncLogger(std::vector<spdlog::sink_ptr> sinks) {
    spdlog::init_thread_pool(1024, 4);
    auto combined_logger = std::make_shared<spdlog::async_logger>(
        "asy_multi_sink", begin(sinks), end(sinks), spdlog::thread_pool());
    return combined_logger;
  };

  std::shared_ptr<spdlog::logger> getLogger(std::vector<spdlog::sink_ptr> sinks) {
    auto combined_logger =
        std::make_shared<spdlog::logger>("multi_sink", begin(sinks), end(sinks));
    return combined_logger;
  };

 public:
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  ~Logger() {
    I_LOG("Logger is shutting down.");
    spdlog::drop_all();
    spdlog::shutdown();
    std::cout << "Logger has been shutdown." << std::endl;
  };

  static void init(const string& logFile = "", bool stdOutOn = true, bool fileOutOn = true, const string& pattern = "", bool useAsyn = true) {
    static bool inited = false;
    if (!inited) {
      static Logger instence{logFile, stdOutOn, fileOutOn, pattern, useAsyn};
      I_LOG("Logger inited success.");
    } else {
      W_LOG("Logger has been inited before, do nothing.");
    }
  };
};

}  // namespace seeker

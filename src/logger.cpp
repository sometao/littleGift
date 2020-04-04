#include "config.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/async.h"

#include <iostream>
#include <memory>
#include <vector>

#ifndef LOG_FILE_NAME
#define LOG_FILE_NAME "application.log"
#endif  // !LOG_FILE_NAME

//short for logger, you may copy following define to your CPP/H file.
#define T_LOG(...) SPDLOG_TRACE(__VA_ARGS__)
#define D_LOG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define I_LOG(...) SPDLOG_INFO(__VA_ARGS__)
#define W_LOG(...) SPDLOG_WARN(__VA_ARGS__)
#define E_LOG(...) SPDLOG_ERROR(__VA_ARGS__)


namespace logger {


void setBaseLogger() {
  auto logFileName = LOG_FILE_NAME;

  auto pattern = "[%Y%m%d %H:%M:%S.%e %s:%#][%!] [%^%L%$]: %v";

  try {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::trace);
    console_sink->set_pattern(pattern);


    auto file_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>(logFileName, 23, 59);
    file_sink->set_pattern(pattern);
    file_sink->set_level(spdlog::level::trace);

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(console_sink);
    sinks.push_back(file_sink);

    spdlog::init_thread_pool(1024, 4);
    auto combined_logger = std::make_shared<spdlog::async_logger>(
        "asy_multi_sink", begin(sinks), end(sinks), spdlog::thread_pool());

    spdlog::set_default_logger(combined_logger);

    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::warn);
    spdlog::flush_every(std::chrono::seconds(2));

  } catch (const spdlog::spdlog_ex& ex) {
    std::cout << "Log initialization failed: " << ex.what() << std::endl;
  }


}


void testLogger() {

  setBaseLogger();

  std::cout << "testLogger BEGIN" << std::endl;
  for (int i = 0; i < 1000; i++) {

    T_LOG("00000---------- {}", i++);
    D_LOG("1111111-------- {}", i++);
    I_LOG("222222222-------{}", i++);
    W_LOG("33333333333-----{}", i++);
    E_LOG("4444444444444---{}", i++);
  }

  std::cout << "testLogger FINISH" << std::endl;
}

void shutdownLogger() {
  spdlog::drop_all();
  spdlog::shutdown();
}

}  // namespace logger
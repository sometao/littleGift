#pragma once
#include "seeker/logger.h"
#include <chrono>


namespace seeker {

using std::string;

class Time {
 public:
  static int64_t currentTime() {
    using namespace std::chrono;
    auto time_now = system_clock::now();
    auto durationIn = duration_cast<milliseconds>(time_now.time_since_epoch());
    return durationIn.count();
  };
};

class String {
public:
  static string toLower(const string& target) {
    string out{};
    for (auto c : target) {
      out += ::tolower(c);
    }
    return out;
  }

  static string toUpper(const string& target) {
    string out{};
    for (auto c : target) {
      out += ::toupper(c);
    }
    return out;
  }
};


}  // namespace seeker
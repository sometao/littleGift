#pragma once
#include "seeker/logger.h"
#include "botan/botan.h"
#include "botan/sha160.h"
#include "botan/sha2_32.h"
#include "botan/md5.h"
#include "botan/hex.h"
#include <string>
#include <sstream>
#include "seeker/common.h"
namespace seeker {
using std::string;

class Secure {
 public:
  static auto uniformIntDistribution(int min, int max, int seed = INT_MIN) {
    static std::random_device rd;
    static std::mt19937 gen{seed == INT_MIN ? rd() : seed};
    static std::uniform_int_distribution<> dis(min, max);
    auto func = [] { return dis(gen); };
    return func;
  };

  static auto uniformDoubleDistribution(double min, double max, int seed = INT_MIN) {
    static std::random_device rd;
    static std::mt19937 gen{seed == INT_MIN ? rd() : seed};
    static std::uniform_real_distribution<> dis(min, max);
    auto func = [] { return dis(gen); };
    return func;
  };

  static auto getCharMap() {
    static char charMap[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                             'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
                             'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
                             'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
                             '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
    return charMap;
  }

  static string randomString(size_t len) {
    static auto dic = getCharMap();
    static auto rng = uniformIntDistribution(0, 10 + 26 + 26, Time::currentTime() % INT_MAX);
    string out{};
    for (int i = 0; i < len; i++) {
      auto r = rng();
      out += dic[r];
    }
    return out;
  }


  static Botan::secure_vector<Botan::byte> process(Botan::MDx_HashFunction& hashFunction,
                                                   const string& target,
                                                   size_t bufSize) {
    if (bufSize > 0 || target.length() > 8192) {
      std::vector<uint8_t> buf(bufSize < 32 ? 4096 : bufSize);
      std::stringstream ss{target};
      while (ss.good()) {
        ss.read(reinterpret_cast<char*>(buf.data()), buf.size());
        const size_t got = static_cast<size_t>(ss.gcount());
        hashFunction.update(buf.data(), got);
      }
      return hashFunction.final();
    } else {
      std::vector<uint8_t> input(target.begin(), target.end());
      return hashFunction.process(input);
    }
  };

  static string sha1(const string& target, size_t bufSize = 0) {
    Botan::SHA_1 hash;
    auto rst = process(hash, target, bufSize);
    auto digital = Botan::hex_encode(rst.data(), rst.size());
    return digital;
  }

  static string sha256(const string& target, size_t bufSize = 0) {
    Botan::SHA_256 hash;
    auto rst = process(hash, target, bufSize);
    auto digital = Botan::hex_encode(rst.data(), rst.size());
    return digital;
  }

  static string md5(const string& target, size_t bufSize = 0) {
    Botan::MD5 hash;
    auto rst = process(hash, target, bufSize);
    auto digital = Botan::hex_encode(rst.data(), rst.size());
    return digital;
  }
};

}  // namespace seeker
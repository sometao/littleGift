#pragma once
#include "seeker/logger.h"
#include "botan/botan_all.h"
#include <string>
#include <sstream>


namespace seeker {
using std::string;

class Secure {
 public:
  Botan::secure_vector<Botan::byte> somefunction(std::vector<Botan::byte> input) {
    Botan::SHA_256 sha;
    return sha.process(input);
  }

  static Botan::secure_vector<Botan::byte> process(Botan::MDx_HashFunction& hashFunction,
                                                   const string& target,
                                                   size_t bufSize = 4096) {
    if (bufSize > 0) {
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

  static string sha256(const string& target, size_t bufSize = 4096) {
    Botan::SHA_256 sha256;
    Botan::MD5 md5;
    auto rst = process(sha256, target, bufSize);
    auto digital = Botan::hex_encode(rst.data(), rst.size());
    return digital;
  }

  static string md5(const string& target, size_t bufSize = 0) {
    Botan::MD5 md5;
    auto rst = process(md5, target, bufSize);
    auto digital = Botan::hex_encode(rst.data(), rst.size());
    return digital;
  }
};

}  // namespace seeker
#include "littleGift.h"
#include "seeker/secure.h"

#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>


namespace littleGift {
namespace test {

  using std::string;
  using std::cout;
  using std::endl;

void do_read_file(std::istream& in, std::function<void(uint8_t[], size_t)> consumer_fn,
                  size_t buf_size) {
  // Avoid an infinite loop on --buf-size=0
  std::vector<uint8_t> buf(buf_size == 0 ? 4096 : buf_size);

  while (in.good()) {
    in.read(reinterpret_cast<char*>(buf.data()), buf.size());
    const size_t got = static_cast<size_t>(in.gcount());
    consumer_fn(buf.data(), got);
  }
}

void read_file(const std::string& input_file,
               std::function<void(uint8_t[], size_t)>
                   consumer_fn,
               size_t buf_size) {
  if (input_file == "-") {
    do_read_file(std::cin, consumer_fn, buf_size);
  } else {
    std::ifstream in(input_file, std::ios::binary);
    if (!in) {
      std::string msg = "reading file error: ";
      msg += input_file;
      throw std::runtime_error(msg);
    }
    do_read_file(in, consumer_fn, buf_size);
  }
}


void read_string(const std::string& target_string,
                 std::function<void(uint8_t[], size_t)>
                     consumer_fn,
                 size_t buf_size) {
  std::stringstream ss{target_string};
  do_read_file(ss, consumer_fn, buf_size);
}



std::string format_blob(const std::string& format, const uint8_t bits[], size_t len) {
#if defined(BOTAN_HAS_HEX_CODEC)
  if (format == "hex") {
    return Botan::hex_encode(bits, len);
  }
#endif

#if defined(BOTAN_HAS_BASE58_CODEC)
  if (format == "base58") {
    return Botan::base58_encode(bits, len);
  }
  if (format == "base58check") {
    return Botan::base58_check_encode(bits, len);
  }
#endif

  // If we supported format, we would have already returned
  throw std::runtime_error("Unknown or unsupported format type");
}

template<typename Alloc>
std::string format_blob(const std::string& format, const std::vector<uint8_t, Alloc>& vec) {
  return format_blob(format, vec.data(), vec.size());
}


void md5Base(const std::string& target) {
  const std::string hash_algo = "MD5";
  const std::string format = "hex";  //"base64"
                                     //const std::string fsname = "";
  const size_t buf_size = 4096;
  std::unique_ptr<Botan::HashFunction> hash_fn(Botan::HashFunction::create(hash_algo));

  //std::unique_ptr<Botan::HashFunction> hash_fn{ Botan::get_hash("MD5") };

  if(!hash_fn)
  {
    auto msg = fmt::format("hashing error: {}", hash_algo);
    E_LOG(msg);
    throw std::runtime_error(msg);
  }

  T_LOG("hashTest [1] ----------------");
  auto update_hash = [&](const uint8_t b[], size_t l) { hash_fn->update(b, l); };
  T_LOG("hashTest [2] ----------------");
  //read_file(fsname, update_hash, buf_size);

  T_LOG("hashTest [3] ----------------");
  read_string(target, update_hash, buf_size);
  T_LOG("hashTest [4] ----------------");

  const std::string digest = format_blob(format, hash_fn->final());
  T_LOG("hashTest [5] ----------------");

  I_LOG("target string: {}", target );
  I_LOG("digest {} string: {}", format, digest );
}


void seekerMd5(const std::string& target) {

  auto digest = seeker::Secure::md5(target, 1024);
  I_LOG("target string: {}", target );
  I_LOG("digest string: {}", digest );
}

void seekerSHA256(const std::string& target) {

  auto digest = seeker::Secure::sha256(target, 1024);
  I_LOG("seekerSHA256 target string: {}", target );
  I_LOG("seekerSHA256 digest string: {}", digest );
}

void hashTest() {
  std::cout << "" << std::endl;
  I_LOG("test hash begin.");

  string target = "aaabbbcccddedf";
  //md5Base(target);
  I_LOG("====================================================");
  //seekerMd5(target);
  seekerSHA256(target);

  

  I_LOG("test hash end.");

}

}  // namespace test


}  // namespace littleGift

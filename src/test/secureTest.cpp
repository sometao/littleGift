#include "seeker/secure.h"



namespace littleGift {
namespace test {

void testRandomInt() {
  auto gen = seeker::Secure::uniformIntDistribution(0, 3);
  for (int i = 0; i < 20; i++) {
    I_LOG("{}\t: value=[{}]", i, gen());
  }
}


void testRandomeStr() {
  for (int i = 0; i < 30; i++) {
    auto str = seeker::Secure::randomChars(4);
    I_LOG("{}\t: value=[{}]", i, str);
  }
}

}  // namespace test

}  // namespace littleGift
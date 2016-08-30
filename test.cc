#include "bit_cast.h"
#include <cstdint>
#include <iostream>

#define T(TO, FROM, INPUT) \
  (std::cout << "bit_cast<" #TO ">((" #FROM ")" << #INPUT << ") -> "	\
   << bit_cast<TO>((FROM)(INPUT)) << '\n')

int main() {
  std::cout << std::hex;
  T(float, uint32_t, 0x00000000);
  T(uint32_t, float, 0.0);
  T(uint32_t, float, -0.0);
  T(uint32_t, float, 2.0);

  std::cout << std::endl;
  return 0;
}

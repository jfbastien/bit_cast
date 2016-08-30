#include "bit_cast.h"
#include <cstdint>
#include <cstdio>
#include <iostream>

size_t failures = 0;

#define T(TO, FROM, INPUT, FMT, EXPECT) do {                            \
    char buf[128] = { '\0' };                                           \
    snprintf(buf, sizeof(buf), FMT, bit_cast<TO>((FROM)(INPUT)));       \
    std::cout                                                           \
      << "bit_cast<" #TO ">((" #FROM ")" #INPUT ")\t"                   \
      << "-> '" << buf << '\'';                                         \
    if (std::string(buf) != std::string(EXPECT)) {                      \
      ++failures;                                                       \
      std::cout << "\texpected: " << (EXPECT);                          \
    }                                                                   \
    std::cout << '\n';                                                  \
  } while (false)

int main() {
  T(float, uint32_t, 0x00000000,     "%a",     "0x0p+0");
  T(uint32_t, float,        0.0, "0x%08x", "0x00000000");
  T(uint32_t, float,       -0.0, "0x%08x", "0x80000000");
  T(uint32_t, float,        2.0, "0x%08x", "0x40000000");

  if (failures)
    std::cout << "Failures: " << failures << std::endl;
  return !!failures;
}

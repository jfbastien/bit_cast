#include "bit_cast.h"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>

size_t failures = 0;

#define T(TO, FROM, INPUT, FMT, EXPECT) do {                            \
    char buf[128] = { '\0' };                                           \
    snprintf(buf, sizeof(buf), FMT, bit_cast<TO>(FROM INPUT));          \
    std::cout                                                           \
      << "bit_cast<" #TO ">(" #FROM " " #INPUT ")\t"                    \
      << "-> '" << buf << '\'';                                         \
    if (std::string(buf) != std::string(EXPECT)) {                      \
      ++failures;                                                       \
      std::cout << "\texpected: " << (EXPECT);                          \
    }                                                                   \
    std::cout << '\n';                                                  \
  } while (false)

struct Float { unsigned mantissa : 23; unsigned exponent : 8; unsigned sign : 1; };
struct NoCtor { NoCtor() = delete; uint32_t u; };

int main() {
  T(   float, uint32_t, (0x00000000),     "%a",     "0x0p+0");
  T(uint32_t,    float,        (0.0), "0x%08x", "0x00000000");
  T(uint32_t,    float,       (-0.0), "0x%08x", "0x80000000");
  T(uint32_t,    float,        (2.0), "0x%08x", "0x40000000");
  T(   float,    Float,    ({0,0,0}),     "%a",     "0x0p+0");
  T(   float,    Float,    ({0,0,1}),     "%a",    "-0x0p+0");
  T(   float,    Float, ({0,0x80,0}),     "%a",     "0x1p+1");

  { float pi = M_PI; std::cout << "PI " << bit_cast<uint32_t>(pi) << '\n'; }
  std::cout << "&main:\t" << bit_cast<uintptr_t>(&main) << '\n';
  { const uint32_t i = 0; std::cout << "const From\t" << bit_cast<float>(i) << '\n'; }
  // const To doesn't make sense.
  // Reference To: use remove_reference?
  // non-constexpr function 'memcpy' cannot be used in a constant expression:
  //   { constexpr uint32_t c = 0; constexpr float f = bit_cast<float>(c); }
  { float f = 0; std::cout << "no ctor\t" << bit_cast<NoCtor>(f).u << '\n'; }
  // Deleting copy ctor or move ctor doesn't make sense when trivially copyable.
  // To being an array type doesn't make sense?

  if (failures)
    std::cout << "Failures: " << failures << std::endl;
  return !!failures;
}

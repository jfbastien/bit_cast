#include "bit_cast.h"
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <typeinfo>

size_t failures = 0;

#define T(TO, FROM, INPUT, ACCESS, FMT, EXPECT) do {                    \
    char buf[128] = { '\0' };                                           \
    snprintf(buf, sizeof(buf), FMT, bit_cast<TO>(FROM INPUT) ACCESS);   \
    std::cout                                                           \
      << "bit_cast<" #TO ">(" #FROM " " #INPUT ")" #ACCESS "\t"         \
      << "-> '" << buf << '\'';                                         \
    if (std::string(buf) != std::string(EXPECT)) {                      \
      ++failures;                                                       \
      std::cout << "\texpected: " << (EXPECT);                          \
    }                                                                   \
    std::cout << '\n';                                                  \
  } while (false)

struct Float { unsigned mantissa : 23; unsigned exponent : 8; unsigned sign : 1; };
struct Padded { uint8_t c; uint16_t s; };
struct NoCtor { NoCtor() = delete; uint32_t u; };
// Deleting copy ctor or move ctor doesn't make sense when trivially copyable.
// Deleting dtor doesn't make sense either.
class Private { uint32_t u; public: uint32_t get() const { return u; } };
struct DefaultMemberInit { uint32_t u = 1337; };
union Union { uint32_t u; float f; };
union UnionNoCtor { struct S { S() = delete; uint32_t u; } s; float f; };
struct StructArray { uint8_t arr[4]; };
struct ZeroWidth { uint32_t u; uint32_t end[0]; };
struct ZeroWidthC { uint32_t u; uint32_t end[]; };
struct Recurse { Recurse() { u = bit_cast<uint32_t>(0.f); } uint32_t u; };
struct RecurseInit { uint32_t u = bit_cast<uint32_t>(0.f); };
struct RecurseAggInit { uint32_t u { bit_cast<uint32_t>(0.f) }; };

int main() {
  //               TO           FROM         INPUT   ACCESS       FMT        EXPECT
  T(            float,      uint32_t, (0x00000000),        ,     "%a",     "0x0p+0");
  T(         uint32_t,         float,        (0.0),        , "0x%08x", "0x00000000");
  T(         uint32_t,         float,       (-0.0),        , "0x%08x", "0x80000000");
  T(         uint32_t,         float,        (2.0),        , "0x%08x", "0x40000000");
  T(   const uint32_t, (const float),        (2.0),        , "0x%08x", "0x40000000");
  T(            float,         Float,    ({0,0,0}),        ,     "%a",     "0x0p+0");
  T(            float,         Float,    ({0,0,1}),        ,     "%a",    "-0x0p+0");
  T(            float,         Float, ({0,0x80,0}),        ,     "%a",     "0x1p+1");
  T(           Padded,         float,        (2.f),      .c, "0x%01x",        "0x0");
  T(           Padded,         float,        (2.f),      .s, "0x%04x",     "0x4000");
  T(           NoCtor,         float,        (2.f),      .u, "0x%08x", "0x40000000");
  T(          Private,         float,        (2.f),  .get(), "0x%08x", "0x40000000");
  T(DefaultMemberInit,         float,        (2.f),      .u, "0x%08x", "0x40000000");
  T(            Union,         float,        (2.f),      .u, "0x%08x", "0x40000000");
  T(      UnionNoCtor,         float,        (2.f),    .s.u, "0x%08x", "0x40000000");
  T(      StructArray,         float,        (2.f), .arr[0], "0x%02x",       "0x00");
  T(      StructArray,         float,        (2.f), .arr[1], "0x%02x",       "0x00");
  T(      StructArray,         float,        (2.f), .arr[2], "0x%02x",       "0x00");
  T(      StructArray,         float,        (2.f), .arr[3], "0x%02x",       "0x40");
  T(        ZeroWidth,         float,        (2.f),      .u, "0x%08x", "0x40000000");
  T(       ZeroWidthC,         float,        (2.f),      .u, "0x%08x", "0x40000000");
  T(          Recurse,         float,        (2.f),      .u, "0x%08x", "0x40000000");
  T(      RecurseInit,         float,        (2.f),      .u, "0x%08x", "0x40000000");
  T(   RecurseAggInit,         float,        (2.f),      .u, "0x%08x", "0x40000000");

  std::cout << "&main as uintptr_t:\t" << std::hex << "0x" << bit_cast<uintptr_t>(&main) << '\n';
  std::cout << "&main as void*:\t" << std::hex << bit_cast<void*>(&main) << '\n';
  std::cout << "&main as " << typeid(&main).name() << ":\t" << std::hex << "0x" << bit_cast<decltype(&main)>(bit_cast<void*>(&main)) << '\n';

  // Reference / rvalue-ref To: could use remove_reference, but it doesn't really make any sense.
  //   uint32_t &v(bit_cast<uint32_t&>(2.f));
  //   uint32_t &&v(bit_cast<uint32_t&&>(2.f));
  //
  // constexpr is currently broken because of memcpy. The standard should
  // specify that constexpr bit_cast "Just Works", it doesn't hinge on memcpy
  // being constexpr but implementations will at least need a compiler builtin.
  // non-constexpr function 'memcpy' cannot be used in a constant expression:
  //   { constexpr uint32_t c = 0; constexpr float f = bit_cast<float>(c); (void)f; }
  //
  // Array To doesn't make sense?
  //
  // std::forward?

  if (failures)
    std::cout << "Failures: " << failures << std::endl;
  return !!failures;
}

#ifndef bit_cast_h
#define bit_cast_h

#include <cstring>
#include <type_traits>

template<typename To, typename From>
To bit_cast(From from) {
  static_assert(sizeof(To) == sizeof(From), "sizes must match");
  static_assert(std::is_trivially_copyable<To>::value, "must be trivially copyable");
  static_assert(std::is_trivially_copyable<From>::value, "must be trivially copyable");
  To to;
  std::memcpy(&to, &from, sizeof(To));
  return to;
}

#endif

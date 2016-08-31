#ifndef bit_cast_h
#define bit_cast_h

#include <cstring>
#include <type_traits>

namespace {

// 1. Requires: `sizeof(To) == sizeof(From)` amd `is_trivially_copyable_v<To>`
//              is `true` and `is_trivially_copyable_v<From>` is `true`.
//
// 2. Returns: an object of type `To` whose *object representation* of *N*
//            `unsigned char` are equal to the object of type `From`'s *object
//            representation* of *N* `unsigned char`. If multiple *value
//            representations* correspond to `To`'s *object representation* then
//            it is implementation-defined which of these *value
//            representations* is returned. If no *value representation*
//            corresponds to `To`'s *object representation* then the returned
//            value is implementation-defined.
template<typename To, typename From>
inline To bit_cast(const From&& from) noexcept {
  static_assert(sizeof(To) == sizeof(From), "sizes must match");
  static_assert(std::is_trivially_copyable<To>::value, "must be trivially copyable");
  static_assert(std::is_trivially_copyable<From>::value, "must be trivially copyable");
  To to{};
  std::memcpy(&to, &from, sizeof(To));
  return std::move(to);
}

}

#endif

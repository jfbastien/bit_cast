#ifndef bit_cast_h
#define bit_cast_h

#include <cstring>
#include <type_traits>
#include <utility>

namespace {

// 1. Requires: `sizeof(To) == sizeof(From)` amd `is_trivially_copyable_v<To>`
//              is `true` and `is_trivially_copyable_v<From>` is `true`.
//
// 2. Returns: an object of type `To` whose *object representation* is equal to
//             the object representation of `From`.
//             If multiple *object representations* could represent the *value
//             representation* of `From`, then it is unspecified which `To` value
//             is returned.
//             If no *value representation* corresponds to `To`'s *object
//             representation* then the returned value is unspecified.
template<typename To, typename From>
inline constexpr To bit_cast(const From&& from) noexcept {
  static_assert(sizeof(To) == sizeof(From), "sizes must match");
  static_assert(std::is_trivially_copyable<To>::value, "must be trivially copyable");
  static_assert(std::is_trivially_copyable<From>::value, "must be trivially copyable");
  To to{};
  std::memcpy(&to, &from, sizeof(To));  // Above `constexpr` is optimistic, fails here.
  return to;
}

}

#endif

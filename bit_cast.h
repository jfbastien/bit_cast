#ifndef BIT_CAST_H
#define BIT_CAST_H

#include <cstring>
#include <type_traits>

// TODO: Use is_trivially_copyable_v when my compiler implements more of C++17.

#if defined(BIT_CAST_USE_CONCEPTS)
# error TODO concepts version not yet implemented
#endif

#if defined(BIT_CAST_USE_SFINAE)
# define BIT_CAST_ENABLE_IF(TO, FROM) ,                                 \
    typename = std::enable_if_t<sizeof(TO) == sizeof(FROM)>,            \
    typename = std::enable_if_t<std::is_trivially_copyable<TO>::value>, \
    typename = std::enable_if_t<std::is_trivially_copyable<FROM>::value>
#else
# define BIT_CAST_ENABLE_IF(TO, FROM)
#endif

#if defined(BIT_CAST_USE_STATIC_ASSERT)
# define BIT_CAST_STATIC_ASSERTS(TO, FROM) do {             \
    static_assert(sizeof(TO) == sizeof(FROM));              \
    static_assert(std::is_trivially_copyable<TO>::value);   \
    static_assert(std::is_trivially_copyable<FROM>::value); \
  } while (false)
#else
# define BIT_CAST_STATIC_ASSERTS(TO, FROM) (void)0
#endif

namespace {

// Defined in header <utility>.
//
// 1. Requires: `sizeof(To) == sizeof(From)` amd `is_trivially_copyable_v<To>`
//              is `true` and `is_trivially_copyable_v<From>` is `true`.
//
// 2. Returns: an object of type `To` whose *object representation* is equal to
//             the object representation of `From`.
//             If multiple *object representations* could represent the *value
//             representation* of `From`, then it is unspecified which `To`
//             value is returned.
//             If no *value representation* corresponds to `To`'s *object
//             representation* then the returned value is unspecified.
template<typename To, typename From BIT_CAST_ENABLE_IF(To, From)>
inline constexpr To bit_cast(const From&& from) noexcept {
  BIT_CAST_STATIC_ASSERTS(To, From);
  std::remove_const_t<To> to{};
  std::memcpy(&to, &from, sizeof(To));  // Above `constexpr` is optimistic, fails here.
  return to;
}

}

#endif

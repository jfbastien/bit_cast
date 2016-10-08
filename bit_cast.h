#ifndef BIT_CAST_H
#define BIT_CAST_H

#include <cstring>
#include <type_traits>

// TODO: Use is_trivially_copyable_v and other type traits when my compiler
//       implements more of C++17.

#if defined(__cpp_concepts) && __cpp_concepts >= 201507
# define BIT_CAST_CONCEPTS(TO, FROM) requires           \
  sizeof(TO) == sizeof(FROM) &&                         \
    std::is_trivially_copyable<TO>::value &&            \
    std::is_trivially_copyable<FROM>::value &&          \
    std::is_standard_layout<TO>::value &&               \
    std::is_standard_layout<FROM>::value &&             \
    !(std::is_pointer<FROM>::value &&                   \
      std::is_pointer<TO>::value) &&                    \
    !(std::is_member_pointer<FROM>::value &&            \
      std::is_member_pointer<TO>::value) &&             \
    !(std::is_member_object_pointer<FROM>::value &&     \
      std::is_member_object_pointer<TO>::value) &&      \
    !(std::is_member_function_pointer<FROM>::value &&   \
      std::is_member_function_pointer<TO>::value)
#else
# define BIT_CAST_CONCEPTS(TO, FROM)
#endif

#if defined(BIT_CAST_USE_SFINAE)
# define BIT_CAST_ENABLE_IF(TO, FROM) ,                                 \
    typename = std::enable_if_t<sizeof(TO) == sizeof(FROM)>,            \
    typename = std::enable_if_t<std::is_trivially_copyable<TO>::value>, \
    typename = std::enable_if_t<std::is_trivially_copyable<FROM>::value>, \
    typename = std::enable_if_t<std::is_standard_layout<TO>::value>,    \
    typename = std::enable_if_t<std::is_standard_layout<FROM>::value>,  \
    typename = std::enable_if_t<!(std::is_pointer<FROM>::value &&       \
                                  std::is_pointer<TO>::value)>,         \
    typename = std::enable_if_t<!(std::is_member_pointer<FROM>::value && \
                                  std::is_member_pointer<TO>::value)>,  \
    typename = std::enable_if_t<!(std::is_member_object_pointer<FROM>::value && \
                                  std::is_member_object_pointer<TO>::value)>, \
    typename = std::enable_if_t<!(std::is_member_function_pointer<FROM>::value && \
                                  std::is_member_function_pointer<TO>::value)>
#else
# define BIT_CAST_ENABLE_IF(TO, FROM)
#endif

#if defined(BIT_CAST_USE_STATIC_ASSERT)
# define BIT_CAST_STATIC_ASSERTS(TO, FROM) do {                         \
    static_assert(sizeof(TO) == sizeof(FROM));                          \
    static_assert(std::is_trivially_copyable<TO>::value);               \
    static_assert(std::is_trivially_copyable<FROM>::value);             \
    static_assert(std::is_standard_layout<TO>::value);                  \
    static_assert(std::is_standard_layout<FROM>::value);                \
    static_assert(!(std::is_pointer<FROM>::value &&                     \
                    std::is_pointer<TO>::value));                       \
    static_assert(!(std::is_member_pointer<FROM>::value &&              \
                    std::is_member_pointer<TO>::value));                \
    static_assert(!(std::is_member_object_pointer<FROM>::value &&       \
                    std::is_member_object_pointer<TO>::value));         \
    static_assert(!(std::is_member_function_pointer<FROM>::value &&     \
                    std::is_member_function_pointer<TO>::value));       \
  } while (false)
#else
# define BIT_CAST_STATIC_ASSERTS(TO, FROM) (void)0
#endif

namespace {

// Defined in header <utility>.
//
// 1. Requires: `sizeof(To) == sizeof(From)`,
//              `is_trivially_copyable_v<To>` is `true`,
//              `is_trivially_copyable_v<From>` is `true`,
//              `is_standard_layout_v<To>` is `true`,
//              `is_standard_layout_v<From>` is `true`,
//              `is_pointer_v<To> && is_pointer_v<From>` is `false`,
//              `is_member_pointer_v<To> && is_member_pointer_v<From>` is `false`,
//              `is_member_object_pointer_v<To> && is_member_object_pointer_v<From>` is `false`,
//              `is_member_function_pointer_v<To> && is_member_function_pointer_v<From>` is `false`.
//
// 2. Returns: an object of type `To` whose *object representation* is equal to
//             the object representation of `From`.
//             If multiple *object representations* could represent the *value
//             representation* of `From`, then it is unspecified which `To`
//             value is returned.
//             If no *value representation* corresponds to `To`'s *object
//             representation* then the returned value is unspecified.
template<typename To, typename From BIT_CAST_ENABLE_IF(To, From)>
BIT_CAST_CONCEPTS(TO, FROM)
inline constexpr To bit_cast(const From& from) noexcept {
  BIT_CAST_STATIC_ASSERTS(To, From);
  typename std::aligned_storage<sizeof(To), alignof(To)>::type storage;
  std::memcpy(&storage, &from, sizeof(To));  // Above `constexpr` is optimistic, fails here.
  return reinterpret_cast<To&>(storage);
  // More common implementation:
  // std::remove_const_t<To> to{};
  // std::memcpy(&to, &from, sizeof(To));  // Above `constexpr` is optimistic, fails here.
  // return to;
}

}

#endif

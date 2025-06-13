#pragma once

#include <cstddef>
#include <array>
#include <type_traits>

namespace traits {
  template<class T>
  struct ArrayStats {
    static constexpr size_t Size = T::size();
    using Type = std::remove_cvref_t<decltype(T()[0])>;
  };
} // namespace traits


namespace traits { // T[N]
  template<class T, size_t N>
  struct ArrayStats<T[N]> {
    static constexpr size_t Size = N;
    using Type = T;
  };
} // namespace traits


namespace traits { // array
  template<class T, size_t N>
  struct ArrayStats<std::array<T, N>> {
    static constexpr size_t Size = N;
    using Type = T;
  };
} // namespace traits


namespace traits {
  template<class Array>
  struct Array2DStats {
    static constexpr size_t N = 
      ArrayStats<Array>::Size;
    static constexpr size_t M = 
      ArrayStats<
        typename ArrayStats<Array>::Type
      >::Size;
    using Type = ArrayStats<
        typename ArrayStats<Array>::Type
      >::Type;
    
  };
} // namespace traits

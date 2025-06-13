#pragma once
#include <cstddef>
#include "../../traits/ArrayStats.h"


// Можно реализовать свертку ссылок
// Типо если b -> a и мы хотим создать c -> b то
// c -> a. И это все будет во время компиляции!

namespace geom::matrix {
  template<class Con, bool IsConst>
  struct LinkContainer {
    using Ptr = std::conditional_t<
      IsConst,
      const Con*,
      Con*
    >;
    using Link = std::conditional_t<
      IsConst,
      const Con&,
      Con&
    >;
    using Stats = traits::Array2DStats<Con>;
    static constexpr size_t N = Stats::N;
    static constexpr size_t M = Stats::M;
    using Type = typename Stats::Type;

    Ptr ptr;
    LinkContainer(Link link):ptr(&link) {}

    auto& operator()(size_t i, size_t j) {
      return (*ptr)[i][j];
    }

    const auto& operator()(size_t i, size_t j) const {
      return (*ptr)[i][j];
    }
  };
} // namespace geom::matrix
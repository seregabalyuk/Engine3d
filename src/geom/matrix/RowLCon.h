#pragma once
#include <cstddef>
#include "../../traits/ArrayStats.h"
#include "../../traits/GetFromPack.h"


namespace geom::matrix {
  template<
    class Row, 
    size_t N, 
    bool IsConst
  > struct RowLCon: 
    RowLCon<Row, N - 1, IsConst> {
   private:
    using _Parant = RowLCon<Row, N - 1, IsConst>;
   public:
    using Ptr = std::conditional_t<
      IsConst,
      const Row*,
      Row*
    >;
    using Link = std::conditional_t<
      IsConst,
      const Row&,
      Row&
    >;
    Ptr rowEnd;
    using Stats = traits::ArrayStats<Row>;
    static constexpr size_t M = Stats::Size;
    using Type = typename Stats::Type;

    template<class... Args>
    RowLCon(Args&&... args):
      _Parant(std::forward<Args>(args)...),
      rowEnd(&(traits::get<N - 1>(
        std::forward<Args>(args)...
      ))) 
    {}
  };
} // namespace geom::matrix


namespace geom::matrix {
  template<
    class Row,
    bool IsConst
  > struct RowLCon<
    Row, 
    0,
    IsConst
  > {
    using Ptr = std::conditional_t<
      IsConst,
      const Row*,
      Row*
    >;
    using Stats = traits::ArrayStats<Row>;
    static constexpr size_t M = Stats::Size;
    using Type = typename Stats::Type;
    template<class... Args>
    RowLCon(Args&&... args) {}

    Ptr* begin() { 
      return reinterpret_cast<Ptr*>(this);
    }

    const Ptr* begin() const { 
      return reinterpret_cast<const Ptr*>(this);
    }

    Type& operator()(size_t i, size_t j) {
      return (*begin()[i])[j];
    }

    const Type& operator() (size_t i, size_t j) const {
      return (*begin()[i])[j];
    }
  };
} // namespace geom::matrix
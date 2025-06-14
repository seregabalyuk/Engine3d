#pragma once
#include <cstddef>
#include "../../traits/ArrayStats.h"
#include "../traits/GetFromPack.h"


namespace geom::matrix {
  template<
    class Row, 
    size_t N, 
    bool isBool
  > struct RowLCon: RowLCon<
    Row, 
    N - 1,
    isBool
  > {
   private:
    using _Parant = RowLCon<Row, N - 1, isBool>;
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

    template<class... Args>
    RowLCon(Args&& args):
      _Parant(std::forward<Args>(args)...),
      rowEnd(&traits::get<N - 1>(
        std::forward<Args>(args)...
      )) 
    {}
    
  };
} // namespace geom::matrix


namespace geom::matrix {
  template<
    class Row,
    bool isBool
  > struct RowLCon<
    Row, 
    0,
    isBool
  > {
    using Ptr = std::conditional_t<
      IsConst,
      const Row*,
      Row*
    >;
    template<class... Args>
    RowLCon(Args&& args) {}

    Ptr* begin() { 
      return reinterpret_cast<Ptr*>(this);
    }

    const Ptr* begin() const { 
      return reinterpret_cast<const Ptr*>(this);
    }

    auto& operator()(size_t i, size_t j) {
      return (begin()[i])->[j];
    }

    const auto& operator()(size_t i, size_t j) const {
      return (begin()[i])->[j];
    }
  };
} // namespace geom::matrix
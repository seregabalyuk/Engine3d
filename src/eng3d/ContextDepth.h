#pragma once
#include "../geom/Triangle.h"

namespace eng3d {
  template<class T, class Context>
  class ContextDepth {
    using Triangle = geom::Triangle<T, 3>;

    T detABC;

    struct Det {
      T all;
      T dx;
      T dy;
      T now;
      T now_y;
    };

    Det detA;
    Det detB;
    Det detC;

    Context* context;

    static void openDet(
      Det& det, 
      const T& dx, 
      const T& dy, 
      const auto& wrap
    ) {
      auto array = geom::det(wrap);
      det.all = array.z - array.x - array.y;
      det.dx = dx * array.x;
      det.dy = dy * array.y;
    }
   public:

    
    ContextDepth(
      const Triangle& triangle,
      size_t width,
      size_t height,
      Context& context
    ): context(& context) {
      T dx = T(2) / width;
      T dy = T(2) / height; 

      auto WrABC = geom::WrapMatrix(triangle);
      detABC = geom::det(WrABC);

      openDet(
        detA,
        dx, dy,
        geom::RowsWrapMatrix(triangle.B, triangle.C)
      );
      openDet(
        detB,
        dx, dy,
        geom::RowsWrapMatrix(triangle.C, triangle.A)
      );
      openDet(
        detC,
        dx, dy,
        geom::RowsWrapMatrix(triangle.A, triangle.B)
      );
    }

    auto getColor() { 
      return context->getColor(
        detA.now, 
        detB.now, 
        detC.now
      ); 
    }

    T getZ() {
      return detABC / (detA.now + detB.now + detC.now);
    }


    void setX(int x) {
      detA.now = detA.now_y + detA.dx * x;
      detB.now = detB.now_y + detB.dx * x;
      detC.now = detC.now_y + detC.dx * x;
    };
    void setY(int y) {
      detA.now_y = detA.all + detA.dy * y;
      detB.now_y = detB.all + detB.dy * y;
      detC.now_y = detC.all + detC.dy * y;
    };
    void incX() {
      detA.now += detA.dx;
      detB.now += detB.dx;
      detC.now += detC.dx;
    };
    void incY() {
      detA.now_y += detA.dy;
      detB.now_y += detB.dy;
      detC.now_y += detC.dy;
    };
  };

} // namespace eng3d
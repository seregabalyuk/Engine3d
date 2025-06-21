#pragma once

#include "../../geom/Line.h"

#include <algorithm>


namespace eng2d {
  template<class T>
  struct Trapezoid {
    using Line = geom::Line<T, 2>;

    Trapezoid(
      int yBegin,
      int yEnd, 
      const Line& left,
      const Line& right
    ): _yBegin(yBegin),
      _yEnd(yEnd),
      _left(&left),
      _right(&right)
    {}

    template<class Surf>
    void draw(
      Surf& surface,
      auto&& context
    ) const {
      int yBegin = std::max(0, _yBegin);
      int yEnd = std::min(_yEnd, (int)surface.height);
      // cut y by _left line
      if (_left->B != 0) { 
        int yPer = _left->y(surface.width - 1);
        if (_left->B < 0) {
          yEnd = std::min(yPer, yEnd);
        } else {
          yBegin = std::max(yPer, yBegin);
        }
      } else if (_left->C >= surface.width * _left-> A) {
        return;
      }

      // cut y by rigth line
      if (_right->B != 0) { 
        int yPer = _right->y(0);
        if (_right->B > 0) {
          yEnd = std::min(yPer, yEnd);
        } else {
          yBegin = std::max(yPer, yBegin);
        }
      } else if (_right->C < 0) {
        return;
      }
      if (yBegin >= yEnd) return;

      auto surfCon = surface.context();
      surfCon.setY(yBegin);
      context.setY(yBegin);

      for (
        int y = yBegin; 
        y < yEnd; 
        ++ y, 
        surfCon.incY(),
        context.incY()
      ) {
        int xBegin = std::max((int)_left->x(y) , 0);
        int xEnd = std::min((int)_right->x(y), (int)surface.width);
        context.setX(xBegin);
        surfCon.setX(xBegin);
        for (
          int x = xBegin; 
          x < xEnd; 
          ++ x,
          context.incX(),
          surfCon.incX()
        ) {
          surfCon.set(context);
        }
        #ifdef DEBUG_TRAPEZOID
        if (xBegin < xEnd) {
          *surface.ptr(xBegin, y) = COLOR(0, 0, 255);
          *surface.ptr(xEnd - 1, y) = COLOR(0, 0, 255);
        }
        #endif
      }

      #ifdef DEBUG_TRAPEZOID_LONGLINES
      {
        auto pixels_from = surface.ptr() + yBegin * surface.width;
        auto pixels_to = pixels_from + surface.width;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 255, 0);

        pixels_from = surface.ptr() + yEnd * surface.width;
        pixels_to = pixels_from + surface.width;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 0, 255);
      }
      #endif

      #ifdef DEBUG_TRAPEZOID
      {
        int xBegin = std::max((int)_left->x(yBegin) , 0);
        int xEnd = std::min((int)_right->x(yBegin), (int)surface.width);
        auto pixels_from = surface.ptr() + 
          yBegin * surface.width;
        auto pixels_to = pixels_from + xEnd;
        pixels_from += xBegin;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 0, 255);
        
        xBegin = std::max((int)_left->x(yEnd - 1) , 0);
        xEnd = std::min((int)_right->x(yEnd - 1), (int)surface.width);
        pixels_from = surface.ptr() + 
          (yEnd - 1) * surface.width;
        pixels_to = pixels_from + xEnd;
        pixels_from += xBegin;
        for(auto it = pixels_from; it < pixels_to; ++ it)
          *it = COLOR(0, 0, 255);
      }
      #endif
    }
    
   private:
    int _yBegin;
    int _yEnd;
    const Line* _left;
    const Line* _right;
  };
} // namespace eng3d
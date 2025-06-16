#pragma once
#include "../geom/Triangle.h"
#include "StackConvex.h"
#include "../geom/Line.h"
#include "ContextDepth.h"
#include "SurfaceZbuffer.h"

namespace eng3d {
  template<class Float = float>
  struct Camera {
    using Point = geom::Vector<Float, 3>;
    using Point2 = geom::Vector<Float, 2>;
    using Matrix = geom::Matrix<Float, 3, 3>;
    using Triangle = geom::Triangle<Float, 3>;
    Matrix matrix;
    Point position;

    Camera(
      const Float& deep,
      const Float& width,
      const Float& height
    ):matrix(0), position(0, 0, 0) {
      matrix(0, 0) = Float(1) / width;
      matrix(1, 1) = Float(1) / height;
      matrix(2, 2) = Float(1) / deep;
    }

    Triangle apply(const Triangle& triangle) const {
      return Triangle(
        (triangle.A - position) * matrix,
        (triangle.B - position) * matrix,
        (triangle.C - position) * matrix
      );
    }

    Point apply(const Point& point) const {
      return (point - position) * matrix;
    }

    static Point2 getByZ(
      const Point& A,
      const Point& B
    ) {
      float a = (Float(1) - B.z) / (A.z - B.z);
      float b = Float(1)  - a;
      return Point2(
        A.x * a + B.x * b,
        A.y * a + B.y * b
      );
    }

    static size_t project(
      const Triangle& triangle,
      Point2 (&convex)[4]
    ) {
      #ifdef NO_CLIPPING
      convex[0] = triangle.A;
      convex[1] = triangle.B;
      convex[2] = triangle.C;
      convex[0] /= triangle.A.z;
      convex[1] /= triangle.B.z;
      convex[2] /= triangle.C.z;
      return 3;
      #endif
      size_t out;
      const auto& A = triangle.A;
      const auto& B = triangle.B;
      const auto& C = triangle.C;
      if (A.z < 1) {
        if (B.z < 1) {
          if (C.z < 1) {
            return 0;
          } else {
            out = 3;
            convex[0] = getByZ(A, C); 
            convex[1] = getByZ(B, C); 
            convex[2] = C;
            convex[2] /= C.z;
          }
        } else {
          if (C.z < 1) {
            out = 3;
            convex[0] = getByZ(A, B); 
            convex[1] = B;
            convex[1] /= B.z;
            convex[2] = getByZ(B, C);
          } else {
            out = 4;
            convex[0] = getByZ(A, B); 
            convex[1] = B;
            convex[1] /= B.z;
            convex[2] = C;
            convex[2] /= C.z;
            convex[3] = getByZ(A, C);
          }
        }
      } else {
        if (B.z < 1) {
          if (C.z < 1) {
            out = 3;
            convex[0] = A; 
            convex[0] /= A.z;
            convex[1] = getByZ(A, B);
            convex[2] = getByZ(A, C);
          } else {
            out = 4;
            convex[0] = A;
            convex[0] /= A.z;
            convex[1] = getByZ(A, B);
            convex[2] = getByZ(B, C);
            convex[3] = C;
            convex[3] /= C.z;
          }
        } else {
          if (C.z < 1) {
            out = 4;
            convex[0] = A;
            convex[0] /= A.z;
            convex[1] = B;
            convex[1] /= B.z;
            convex[2] = getByZ(B, C);
            convex[3] = getByZ(A, C);
          } else {
            out = 3;
            convex[0] = A; 
            convex[0] /= A.z;
            convex[1] = B;
            convex[1] /= B.z;
            convex[2] = C;
            convex[2] /= C.z;
          }
        }
      }
      if (cross(
        convex[0] - convex[1], 
        convex[1] - convex[2]
      ) > 0) {
        return 0;
      }
      return out;
    }

    template<class Pixel>
    static void draw(
      SurfaceZbuffer<Pixel, Float>& surface,
      const Triangle& triangle,
      auto&& context
    ) {
      geom::Vector<Float, 2> points[4];
      size_t count = project(triangle, points);
      if (count == 0) return;

      geom::Vector shift(1.f, 1.f);
      float w = surface.width / 2;
      float h = surface.height / 2;
      for (size_t i = 0; i < count; ++ i) {
        points[i] += shift;
        points[i].x *= w;
        points[i].y *= h;
      }
      ContextDepth depth(
        triangle, 
        surface.width, 
        surface.height, 
        context
      );

      if (count == 3) {
        auto& convex = reinterpret_cast<
          eng3d::StackConvex<Float, 3>&
        >(points);
        convex.normalizeLight();      
        surface.draw(
          convex,
          depth
        );
      } else if (count == 4) {
        auto& convex = reinterpret_cast<
          eng3d::StackConvex<Float, 4>&
        >(points);
        convex.normalizeLight();      
        surface.draw(
          convex,
          depth
        );
      }
    }


    template<class Pixel>
    static void draw(
      Surface<Pixel>& surface,
      const Triangle& triangle,
      auto&& context
    ) {
      geom::Vector<Float, 2> points[4];
      size_t count = project(triangle, points);
      if (count == 0) return;

      geom::Vector shift(1.f, 1.f);
      float w = surface.width / 2;
      float h = surface.height / 2;
      for (size_t i = 0; i < count; ++ i) {
        points[i] += shift;
        points[i].x *= w;
        points[i].y *= h;
      }

      if (count == 3) {
        auto& convex = reinterpret_cast<
          eng3d::StackConvex<Float, 3>&
        >(points);
        convex.normalizeLight();      
        surface.draw(
          convex,
          context
        );
      } else if (count == 4) {
        auto& convex = reinterpret_cast<
          eng3d::StackConvex<Float, 4>&
        >(points);
        convex.normalizeLight();      
        surface.draw(
          convex,
          context
        );
      }
    }
  };
}
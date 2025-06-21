#pragma once

#include "../geom/Triangle.h"
#include "StackConvex.h"
#include "../geom/Line.h"
#include "CameraContext.h"


namespace eng3d {
  template<class Float = float>
  struct Camera {
    using Point = geom::Vector<Float, 3>;
    using Point2 = geom::Vector<Float, 2>;
    using Matrix = geom::Matrix<Float, 3, 3>;
    using Triangle = geom::Triangle<Float, 3>;
    using Convex = eng2d::SimpleConvex<Float>;
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

    static Convex project(
      const Triangle& triangle,
      Point2 (&buffer)[4]
    ) {
      #ifdef NO_CLIPPING
      buffer[0] = triangle.A;
      buffer[1] = triangle.B;
      buffer[2] = triangle.C;
      buffer[0] /= triangle.A.z;
      buffer[1] /= triangle.B.z;
      buffer[2] /= triangle.C.z;
      return Convex(buffer, 3);
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
            buffer[0] = getByZ(A, C); 
            buffer[1] = getByZ(B, C); 
            buffer[2] = C;
            buffer[2] /= C.z;
          }
        } else {
          if (C.z < 1) {
            out = 3;
            buffer[0] = getByZ(A, B); 
            buffer[1] = B;
            buffer[1] /= B.z;
            buffer[2] = getByZ(B, C);
          } else {
            out = 4;
            buffer[0] = getByZ(A, B); 
            buffer[1] = B;
            buffer[1] /= B.z;
            buffer[2] = C;
            buffer[2] /= C.z;
            buffer[3] = getByZ(A, C);
          }
        }
      } else {
        if (B.z < 1) {
          if (C.z < 1) {
            out = 3;
            buffer[0] = A; 
            buffer[0] /= A.z;
            buffer[1] = getByZ(A, B);
            buffer[2] = getByZ(A, C);
          } else {
            out = 4;
            buffer[0] = A;
            buffer[0] /= A.z;
            buffer[1] = getByZ(A, B);
            buffer[2] = getByZ(B, C);
            buffer[3] = C;
            buffer[3] /= C.z;
          }
        } else {
          if (C.z < 1) {
            out = 4;
            buffer[0] = A;
            buffer[0] /= A.z;
            buffer[1] = B;
            buffer[1] /= B.z;
            buffer[2] = getByZ(B, C);
            buffer[3] = getByZ(A, C);
          } else {
            out = 3;
            buffer[0] = A; 
            buffer[0] /= A.z;
            buffer[1] = B;
            buffer[1] /= B.z;
            buffer[2] = C;
            buffer[2] /= C.z;
          }
        }
      }
      if (cross(
        buffer[0] - buffer[1], 
        buffer[1] - buffer[2]
      ) > 0) {
        return Convex(buffer, 0);
      }
      return Convex(buffer, out);
    }

    template<class...Shaders>
    static void draw(
      auto& surface,
      const Triangle& triangle,
      Shaders&&... shaders
    ) {
      Point2 points[4];
      auto convex = project(triangle, points);
      if (convex.size() == 0) return;

      Point2 shift(1.f, 1.f);
      float w = surface.width / 2;
      float h = surface.height / 2;
      for (auto& point: convex) {
        point += shift;
        point.x *= w;
        point.y *= h;
      }
      CameraContext context(
        triangle,
        surface.width, 
        surface.height, 
        std::forward<Shaders>(shaders)...
      );
      convex.normalizeLight();
      surface.draw(
        convex,
        context
      );
    }
  };
}
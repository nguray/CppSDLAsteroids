#pragma once
#include <math.h>

class RVector2D {
public:
  float x;
  float y;

  RVector2D(float x = 0.0f, float y = 0.0f);

  float magnitude();
  void normalize();
  static RVector2D normalize(RVector2D v);
  RVector2D normal();

  friend RVector2D operator+(RVector2D A, const RVector2D &B) { return A += B; }

  RVector2D &operator+=(const RVector2D &B) {
    x += B.x;
    y += B.y;
    return *this;
  }

  friend RVector2D operator-(RVector2D A, const RVector2D &B) { return A -= B; }

  RVector2D &operator-=(const RVector2D &B) {
    x -= B.x;
    y -= B.y;
    return *this;
  }

  friend RVector2D operator*(float left, const RVector2D &right) {
    return RVector2D(left * right.x, left * right.y);
  }

  friend RVector2D operator*(const RVector2D &left, float rhs) {
    return RVector2D(left.x * rhs, left.y * rhs);
  }

  // Dot product
  friend float operator*(const RVector2D &A, const RVector2D &B) {
    return A.x * B.x + A.y * B.y;
  }

  float distance(const RVector2D &a);

  static float distance(const RVector2D &a, const RVector2D &b);

  void mul(float v);

  float dot(RVector2D v);
};
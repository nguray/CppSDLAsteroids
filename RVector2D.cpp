
#include "RVector2D.h"

RVector2D::RVector2D(float x, float y) : x(x), y(y) {}

// Get magnitude of vector
float RVector2D::magnitude() { return std::sqrt(x * x + y * y); }

// Get a unit vector in the direction of this vector
// If this vector is the 0 vector, return a 0 vector
RVector2D RVector2D::normalize() {
  double mag = magnitude();
  if (mag != 0.)
    return RVector2D(x / mag, y / mag);
  else
    return RVector2D();
}

// RVector2D RVector2D::normalize(RVector2D v) {
//   double mag = std::sqrt(v.x * v.x + v.y * v.y);
//   if (mag != 0.)
//     return RVector2D(v.x / mag, v.y / mag);
//   else
//     return RVector2D();
// }

RVector2D RVector2D::normal() { return RVector2D(y, -x); }

// Distance
float RVector2D::distance(const RVector2D &a, const RVector2D &b) {
  auto v = a - b;
  return v.magnitude();
}

float RVector2D::distance(const RVector2D &a) {
  auto v = *this - a;
  return v.magnitude();
}

void RVector2D::mul(float v) {
  x *= v;
  y *= v;
}

float RVector2D::dot(RVector2D v) { return x * v.x + y * v.y; }
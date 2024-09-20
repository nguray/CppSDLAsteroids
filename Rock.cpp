#include "Rock.h"
#include <math.h>

float Rock::sav_cos[] = {};
float Rock::sav_sin[] = {};

Rock::Rock() : GameObject(), iExplode(0) {}

Rock::Rock(RVector2D p, RVector2D v, float m) : GameObject(), iExplode(0) {
  pos = p;
  velocity = v;
  mass = m;
  radius = 10.0 * m;
}

Rock::~Rock() {}

void Rock::UpdatePosition() { pos += velocity; }

void Rock::Draw(SDL_Renderer *renderer) {
  //--
  if (iExplode == 0) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawCircle(renderer, static_cast<int>(pos.x),
                         static_cast<int>(pos.y), radius);
    auto p1 = pos;
    auto v = velocity;
    v.mul(10);
    auto p2 = p1 + v;
    SDL_RenderDrawLineF(renderer, p1.x, p1.y, p2.x, p2.y);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (int i = 0; i < NB_COSSINS; ++i) {
      auto p1 = *points[i];
      auto v = explVecs[i]->normalize();
      v.mul(static_cast<float>(iExplode));
      auto p2 = p1 + v;
      SDL_RenderDrawLineF(renderer, p1.x, p1.y, p2.x, p2.y);
    }
  }
}

void Rock::InitExplosion() {
  for (int i = 0; i < NB_COSSINS; ++i) {
    auto v = std::make_shared<RVector2D>(2.0f * sav_cos[i], 2.0f * sav_sin[i]);
    explVecs.push_back(v);
    auto p = pos + *v;
    points.push_back(std::make_shared<RVector2D>(p.x, p.y));
  }
}

void Rock::UpdateExplosion() {
  for (int i = 0; i < NB_COSSINS; ++i) {
    auto v = velocity * 3;
    auto p = *points[i] + v;
    p += *explVecs[i];
    points[i]->x = p.x;
    points[i]->y = p.y;
  }
}

void Rock::InitCosSinValues() {
  //--
  double aOffset = (360.0 / NB_COSSINS) * M_PI / 180.0;
  for (int i = 0; i < NB_COSSINS; ++i) {
    double ra = i * aOffset;
    sav_cos[i] = cos(ra);
    sav_sin[i] = sin(ra);
  }
}
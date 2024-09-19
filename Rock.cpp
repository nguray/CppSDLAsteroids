#include "Rock.h"
#include <math.h>

extern float rcos[NB_COSSINS];
extern float rsin[NB_COSSINS];

Rock::Rock() : GameObject(), iExplode(0) {}

Rock::Rock(RVector2D p, RVector2D v, float m) : GameObject(), iExplode(0) {
  pos = p;
  velocity = v;
  mass = m;
  radius = 10.0 * m;
}

Rock::~Rock() {
  for (auto p : points) {
    delete p;
  }
  points.clear();
  for (auto v : explVecs) {
    delete v;
  }
  explVecs.clear();
}

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
    auto v = new RVector2D{2.0f * rcos[i], 2.0f * rsin[i]};
    explVecs.push_back(v);
    auto p = pos + *v;
    points.push_back(new RVector2D{p.x, p.y});
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
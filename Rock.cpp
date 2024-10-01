/*---------------------------------------------------------------------------*\


\*---------------------------------------------------------------------------*/
#include "Rock.h"
#include <math.h>

float Rock::sav_cos[] = {};
float Rock::sav_sin[] = {};

int Rock::RandomInt(int a, int b)
{
  int diff = b - a + 1;
  int r = rand() % diff;
  return a + r;
}

Rock::Rock() : GameObject(), iExplode(0), curTex(NULL) {}

Rock::Rock(RVector2D p, RVector2D v, float m) : GameObject(), iExplode(0), curTex(NULL) {
  pos = p;
  velocity = v;
  mass = m;
  radius = 10.0 * m;
  if (m == 2) {
    rotateSpeed = static_cast<float>(RandomInt(1, 9)) / 10.0f;
  }
  else {
    rotateSpeed = static_cast<float>(RandomInt(10, 19)) / 10.0f;
  }
  auto i = RandomInt(1, 2);
  if (i == 2) {
    rotateSpeed = -rotateSpeed;
  }
}

Rock::~Rock() {}

void Rock::UpdatePosition() {
  pos += velocity;
  direction += rotateSpeed;
}

void Rock::Draw(SDL_Renderer* renderer) {
  //--
  if (iExplode == 0) {

    if (curTex != nullptr) {

      SDL_Rect desRect = { static_cast<int>(pos.x - curTex->width / 2 - 1),static_cast<int>(pos.y - curTex->height / 2 - 1), 64, 64 };
      SDL_RenderCopyEx(renderer, curTex->texture, nullptr, &desRect, direction, NULL, SDL_RendererFlip::SDL_FLIP_NONE);

      // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      // SDL_RenderDrawCircle(renderer, static_cast<int>(pos.x),
      //   static_cast<int>(pos.y), radius);

    }
    else {
      SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
      SDL_RenderDrawCircle(renderer, static_cast<int>(pos.x),
        static_cast<int>(pos.y), radius);
      auto p1 = pos;
      auto v = velocity;
      v.mul(10);
      auto p2 = p1 + v;
      SDL_RenderDrawLineF(renderer, p1.x, p1.y, p2.x, p2.y);

    }

  }
  else {
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    for (int i = 0; i < NB_COSSINS; ++i) {
      auto p1 = *points[i];
      auto v = RVector2D::normalize(*explVecs[i]);
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

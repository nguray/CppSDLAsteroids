#include "Bullet.h"

Bullet::Bullet(RVector2D p, RVector2D v) : GameObject() {
  pos = p;
  velocity = v;
  radius = 2;
}

Bullet::~Bullet() {}

void Bullet::UpdatePosition() { pos += velocity; }

bool Bullet::Hit(std::shared_ptr<Rock> r) {
  //--
  auto v = pos - r->pos;
  auto d = v.magnitude();
  return d < (r->radius + radius);
}

void Bullet::Draw(SDL_Renderer *renderer) {
  //--
  auto uv = velocity.normalize();
  auto tv = uv.normal();

  uv.mul(5.0);
  tv.mul(2.0);

  auto x1 = pos.x;
  auto y1 = pos.y;
  auto x2 = x1 - uv.x + tv.x;
  auto y2 = y1 - uv.y + tv.y;
  auto x3 = x1 - uv.x - tv.x;
  auto y3 = y1 - uv.y - tv.y;

  SDL_FPoint points[4] = {SDL_FPoint{x1, y1}, SDL_FPoint{x2, y2},
                          SDL_FPoint{x3, y3}, SDL_FPoint{x1, y1}};

  SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  SDL_RenderDrawLinesF(renderer, points, 4);
}
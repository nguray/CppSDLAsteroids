#pragma once
#include "GameObject.h"
#include "RVector2D.h"
#include "Rock.h"
#include <SDL.h>
#include <memory>

class Bullet : public GameObject {
public:
  Bullet(RVector2D p, RVector2D v);
  ~Bullet();

  void Draw(SDL_Renderer *renderer) override;
  void UpdatePosition() override;
  bool Hit(std::shared_ptr<Rock> r);
};
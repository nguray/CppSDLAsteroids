#pragma once
#include "GameObject.h"
#include "RVector2D.h"
#include <SDL.h>
#include <memory>
#include <vector>

class Rock : public GameObject {
private:
#define NB_COSSINS 6
  static float sav_cos[6];
  static float sav_sin[6];

public:
  Rock();
  Rock(RVector2D p, RVector2D v, float m);
  ~Rock();

  void Draw(SDL_Renderer* renderer) override;
  void UpdatePosition() override;
  void InitExplosion();
  void UpdateExplosion();

  static void InitCosSinValues();

  int iExplode;
  std::vector<std::shared_ptr<RVector2D>> explVecs;
  std::vector<std::shared_ptr<RVector2D>> points;

};
#pragma once
#include "GameObject.h"
#include <SDL.h>
#include <vector>

#define NB_COSSINS 6

class Rock : public GameObject {

public:
  Rock();
  Rock(RVector2D p, RVector2D v, float m);
  ~Rock();

  void Draw(SDL_Renderer *renderer) override;
  void UpdatePosition() override;
  void InitExplosion();
  void UpdateExplosion();

  static void PreCalculateCosSin();

  int iExplode;

  std::vector<RVector2D *> explVecs;
  std::vector<RVector2D *> points;
};
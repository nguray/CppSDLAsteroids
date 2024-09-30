#pragma once
#include "GameObject.h"
#include "RVector2D.h"
#include <SDL.h>
#include <memory>
#include <vector>
#include "RTexture.h"

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
  int  RandomInt(int a, int b);

  static void InitCosSinValues();

  int iExplode;
  float rotateSpeed;
  std::vector<std::shared_ptr<RVector2D>> explVecs;
  std::vector<std::shared_ptr<RVector2D>> points;
  RTexture* curTex;

};
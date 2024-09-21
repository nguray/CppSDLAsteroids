#pragma once
#include <SDL.h>
#include "RVector2D.h"
#include "GameObject.h"

class Ship : public GameObject {

public:
    Ship();
    Ship(RVector2D p, float a);
    ~Ship();

    void Draw(SDL_Renderer* renderer) override;
    void UpdatePosition() override;

    void SetAngle(float a);
    void OffsetAngle(float da);
    void Accelerate(float d);
    void DecSheild();
    void IncSheild();

    void SetIdleThrush();
    void SetForwardThrush();
    void SetBackwardThrush();
    RVector2D DirectionVect();

    int shieldLevel;
    SDL_Texture* curTex;
    SDL_Texture* idleTex;
    SDL_Texture* accelTex;
    SDL_Texture* decelTex;

};
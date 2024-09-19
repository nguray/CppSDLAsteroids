#pragma once
#include <SDL.h>
#include "RVector2D.h"
#include "GameObject.h"

class Ship : public GameObject {

public:
    Ship();
    Ship(RVector2D p, float a);
    ~Ship();

    void Draw(SDL_Renderer *renderer) override;
    void UpdatePosition() override;

    void SetAngle(float a);
    void OffsetAngle(float da);
    void Accelerate(float d);

    void SetIdleThrush();
    void SetForwardThrush();
    void SetBackwardThrush();
    RVector2D DirectionVect();


    // RVector2D   pos;
    // RVector2D   velocity;
    // float       direction;
    // float       mass;
    // float       radius;
    // RVector2D   thrushUnit;

    SDL_Texture* curTex;
    SDL_Texture* idleTex;
    SDL_Texture* accelTex;
    SDL_Texture* decelTex;

};
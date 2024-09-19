#pragma once
#include <SDL.h>
#include "RVector2D.h"

class GameObject {

public:
    GameObject();
    ~GameObject();

    virtual void Draw(SDL_Renderer *renderer)=0;
    virtual void UpdatePosition()=0;

    int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);

    RVector2D   pos;
    RVector2D   velocity;
    float       direction;
    float       mass;
    float       radius;
    RVector2D   thrushUnit;
    bool        fDelete;

};
/*---------------------------------------------------------------------------*\


\*---------------------------------------------------------------------------*/
#include "Ship.h"
#include <math.h>

Ship::Ship() : GameObject(),
curTex(NULL),
idleTex(NULL),
accelTex(NULL),
decelTex(NULL),
shieldLevel(3)
{

}

Ship::Ship(RVector2D p, float a) : GameObject(),
curTex(NULL),
idleTex(NULL),
accelTex(NULL),
decelTex(NULL),
shieldLevel(3)
{
    pos = p;
    direction = a;
    double ra = a * M_PI / 180.0;
    thrushUnit = RVector2D{ static_cast<float>(cos(ra)),static_cast<float>(sin(ra)) };
    velocity = thrushUnit * 0.1;
    mass = 2.0;
    radius = (8.0 + static_cast<float>(shieldLevel)) * mass;
}

Ship::~Ship() {

}

void Ship::DecSheild()
{
    if (shieldLevel) {
        shieldLevel--;
        radius = (8.0 + static_cast<float>(shieldLevel)) * mass;
    }
}

void Ship::IncSheild()
{
    if (shieldLevel < 3) {
        shieldLevel++;
        radius = (8.0 + static_cast<float>(shieldLevel)) * mass;
    }
}

void Ship::UpdatePosition()
{
    pos += velocity;
}

void Ship::Accelerate(float d)
{
    auto v = thrushUnit;
    v.mul(d);
    velocity += v;
}

void Ship::Draw(SDL_Renderer* renderer)
{
    //--
    int red = (255 - shieldLevel * 64);
    SDL_SetRenderDrawColor(renderer, red, 64, 64, 128);

    SDL_RenderDrawCircle(renderer, static_cast<int>(pos.x), static_cast<int>(pos.y), radius);

    // auto r = radius - 3.0f;
    // for (int i = 0; i < (shieldLevel + 1); i++)
    // {
    //     SDL_RenderDrawCircle(renderer, static_cast<int>(pos.x), static_cast<int>(pos.y), r);
    //     r -= 1.0;
    // }

    SDL_Rect srcRect = { 0,0, 32, 32 };
    SDL_Rect desRect = { static_cast<int>(pos.x) - 15,static_cast<int>(pos.y - 15), 32, 32 };
    SDL_RenderCopyEx(renderer, curTex, &srcRect, &desRect, direction + 90.0, NULL, SDL_RendererFlip::SDL_FLIP_NONE);

}

void Ship::SetAngle(float a) {
    direction = a;
    auto ra = (a * M_PI) / 180.0;
    thrushUnit = RVector2D{ static_cast<float>(cos(ra)),static_cast<float>(sin(ra)) };
}

void Ship::OffsetAngle(float da) {
    SetAngle(direction + da);
}

void Ship::SetIdleThrush() {
    curTex = idleTex;
}

void Ship::SetForwardThrush() {
    curTex = accelTex;
}

void Ship::SetBackwardThrush() {
    curTex = decelTex;
}

RVector2D Ship::DirectionVect() {
    auto ra = static_cast<float>(direction * M_PI / 180.0);
    return RVector2D{ cos(ra),sin(ra) };
}
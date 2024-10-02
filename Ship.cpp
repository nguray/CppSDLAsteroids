/*---------------------------------------------------------------------------*\


\*---------------------------------------------------------------------------*/
#include "Ship.h"
#include <math.h>
#include <filesystem>
#include <SDL_image.h>

Ship::Ship(SDL_Renderer* renderer) : GameObject(),
curTex(NULL),
idleTex(NULL),
accelTex(NULL),
decelTex(NULL),
shieldLevel(3),
renderer(renderer)
{
    std::filesystem::path resDir;
    if (std::filesystem::exists("../resources")) {
        resDir = std::filesystem::path("../resources");
    }
    else {
        resDir = std::filesystem::path("./resources");
    }

    //--
    auto filePath = resDir / "Plane00.png";
    // Load image at specified path
    SDL_Surface* shipdSurface0 = IMG_Load(filePath.c_str());

    filePath = resDir / "Plane01.png";
    // Load image at specified path
    SDL_Surface* shipdSurface1 = IMG_Load(filePath.c_str());

    filePath = resDir / "Plane02.png";
    // Load image at specified path
    SDL_Surface* shipdSurface2 = IMG_Load(filePath.c_str());

    idleTex = SDL_CreateTextureFromSurface(renderer, shipdSurface0);
    accelTex = SDL_CreateTextureFromSurface(renderer, shipdSurface1);
    decelTex = SDL_CreateTextureFromSurface(renderer, shipdSurface2);
    curTex = idleTex;

    //--
    SDL_FreeSurface(shipdSurface0);
    SDL_FreeSurface(shipdSurface1);
    SDL_FreeSurface(shipdSurface2);

    mass = 1.7f;
    radius = (8.0 + static_cast<float>(shieldLevel)) * mass;

}

Ship::~Ship() {
    //--
    SDL_DestroyTexture(idleTex);
    SDL_DestroyTexture(accelTex);
    SDL_DestroyTexture(decelTex);

}

void Ship::SetPosition(const RVector2D& newPos) {
    pos = newPos;
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

    SDL_Rect desRect = { static_cast<int>(pos.x) - 15,static_cast<int>(pos.y - 15), 32, 32 };
    SDL_RenderCopyEx(renderer, curTex, nullptr, &desRect, direction + 90.0, NULL, SDL_RendererFlip::SDL_FLIP_NONE);

}

void Ship::SetDirection(float a) {
    direction = a;
    auto ra = (a * M_PI) / 180.0;
    thrushUnit = RVector2D{ static_cast<float>(cos(ra)),static_cast<float>(sin(ra)) };
}

void Ship::OffsetAngle(float da) {
    SetDirection(direction + da);
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
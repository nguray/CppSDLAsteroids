/*---------------------------------------------------------------------------*\


\*---------------------------------------------------------------------------*/

#include "RTexture.h"

RTexture::RTexture(SDL_Texture* texture) :
    texture(texture)
{
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
}

RTexture::~RTexture()
{
    SDL_DestroyTexture(texture);
}
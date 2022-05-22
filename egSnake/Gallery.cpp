#include "Gallery.h"

Gallery::Gallery(Painter& painter_)
    : painter(painter_)
{
    loadGamePictures();
}

Gallery::~Gallery()
{
    for (SDL_Texture* texture : pictures)
        SDL_DestroyTexture(texture);
}

void Gallery::loadGamePictures()
{
    pictures.push_back(painter.loadTexture("pre_picture.jpg"));
    pictures.push_back(painter.loadTexture("back_ground.jpg"));
    pictures.push_back(painter.loadTexture("cherry.jpg"));
    pictures.push_back(painter.loadTexture("snake_vertical.jpg"));
    pictures.push_back(painter.loadTexture("snake_horizontal.png"));
    pictures.push_back(painter.loadTexture("snake_head.jpg"));
    pictures.push_back(painter.loadTexture("af_picture.jpg"));
}

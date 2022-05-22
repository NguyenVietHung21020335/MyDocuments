#include <cmath>
#include <cstdlib>
#include <iostream>

#include <SDL_image.h>

#include "painter.h"

using namespace std;

//Khởi tạo
Painter::Painter(SDL_Window* window, SDL_Renderer* renderer_)
    : renderer(renderer_)
{
    SDL_RenderGetLogicalSize(renderer, &width, &height);
    if (width == 0 && height == 0)
        SDL_GetWindowSize(window, &width, &height); //Lấy kích thước cửa sổ
    //Khởi tạo mặc định tọa độ,	màu và hướng ban đầu của bút vẽ, tô nền bằng màu xanh
    setPosition(width/2, height/2);
    setAngle(0);
    setColor(WHITE_COLOR);
    clearWithBgColor(BLUE_COLOR);
}

void Painter::setPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Painter::setAngle(float angle)
{
    this->angle = angle - floor(angle/360)*360;
}

void Painter::setColor(SDL_Color color)
{
    this->color = color;
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 0);
}

void Painter::clearWithBgColor(SDL_Color bgColor)
{
    SDL_Color curColor = color;
    setColor(bgColor);
    SDL_RenderClear(renderer);
    setColor(curColor);
}

void Painter::moveForward(float length) //hàm di chuyển tiến lên đoạn bằng length
{
    float prevX = x, prevY = y;
    jumpForward(length);
    SDL_RenderDrawLine(renderer, (int)prevX, (int)prevY, (int)x, (int)y); //hàm vẽ đường từ điểm cũ đến điểm mới
}

void Painter::jumpForward(float length)
{
    float rad = (angle / 180) * M_PI; //chuyển sang radian để dùng cos sin
    x += (cos(rad) * length);
    y -= (sin(rad) * length);
}

void Painter::createCircle(float radius)
{
    double rad = (angle / 180) * M_PI;
    int centerX = x + cos(rad) * radius;
    int centerY = y - sin(rad) * radius;

    int dx = radius;
    int dy = 0;
    int err = 0;

    while (dx >= dy)
    {
        SDL_RenderDrawPoint(renderer, centerX + dx, centerY + dy);
        SDL_RenderDrawPoint(renderer, centerX + dy, centerY + dx);
        SDL_RenderDrawPoint(renderer, centerX - dy, centerY + dx);
        SDL_RenderDrawPoint(renderer, centerX - dx, centerY + dy);
        SDL_RenderDrawPoint(renderer, centerX - dx, centerY - dy);
        SDL_RenderDrawPoint(renderer, centerX - dy, centerY - dx);
        SDL_RenderDrawPoint(renderer, centerX + dy, centerY - dx);
        SDL_RenderDrawPoint(renderer, centerX + dx, centerY - dy);

        if (err <= 0)
        {
            dy += 1;
            err += 2*dy + 1;
        }
        if (err > 0)
        {
            dx -= 1;
            err -= 2*dx + 1;
        }
    }
}

void Painter::createSquare(float size)
{
    for (int i = 0; i < 4; ++i) {
        moveForward(size);
        turnLeft(90);
    }
}

void Painter::createParallelogram(float size)
{
    for (int i = 0; i < 2; ++i) {
        moveForward(size);
        turnLeft(60);
        moveForward(size);
        turnLeft(120);
    }
}

SDL_Texture* Painter::loadTexture( string path ) // tham khảo file lazyfoo bài 7 mình đã học  https://drive.google.com/file/d/1rn3nmVW402bzhw_rnBQVtFKNaAoOs3MJ/view?usp=sharing
{
    //Final texture
    SDL_Texture* newTexture = NULL;
    //Tải ảnh bằng đường dẫn chỉ định
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if ( loadedSurface == NULL )
        cout << "Unable to load image " << path << " SDL_image Error: " << IMG_GetError() << endl;
    else {
        //Tạo texture từ surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
        if( newTexture == NULL )
            cout << "Unable to create texture from " << path << " SDL Error: " << SDL_GetError() << endl;
        /*
		Chức năng tải kết cấu của chúng tôi trông gần giống như trước đây chỉ bây giờ thay vì chuyển đổi bề mặt được tải sang định dạng hiển thị,
		chúng tôi tạo kết cấu từ bề mặt được tải bằng cách sử dụng SDL_CreateTextureFromSurface .
		Giống như trước đây, chức năng này tạo ra một kết cấu mới từ một bề mặt hiện có, có nghĩa là trước khi chúng ta phải giải phóng bề mặt đã tải và sau đó trả lại kết cấu đã tải
		.*/
		//Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    return newTexture;
}

bool Painter::createImage( SDL_Texture* texture, SDL_Rect* srcrect, SDL_Rect* dstrect)
{
    if( texture == NULL ) return false;
    SDL_RenderCopy( renderer, texture, srcrect, dstrect ); //Hàm hiển thị ảnh đó ra màn hình
    return true;
}

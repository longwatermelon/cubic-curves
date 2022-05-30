#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_FPoint bezier(SDL_Point points[4], float t)
{
    float b[4] = {
        powf(1 - t, 3.f),
        3 * t * powf(1 - t, 2.f),
        3 * powf(t, 2.f) * (1 - t),
        powf(t, 3.f)
    };

    SDL_FPoint ret = { 0.f, 0.f };

    for (int i = 0; i < 4; ++i)
    {
        ret.x += b[i] * points[i].x;
        ret.y += b[i] * points[i].y;
    }

    return ret;
}

SDL_FPoint bspline(SDL_Point points[4], float t)
{
    float b[4] = {
        (1.f / 6.f) * powf(1 - t, 3.f),
        (1.f / 6.f) * (3 * powf(t, 3.f) - 6 * powf(t, 2.f) + 4),
        (1.f / 6.f) * (-3 * powf(t, 3.f) + 3 * powf(t, 2.f) + 3 * t + 1),
        (1.f / 6.f) * powf(t, 3.f)
    };

    SDL_FPoint ret = { 0.f, 0.f };

    for (int i = 0; i < 4; ++i)
    {
        ret.x += b[i] * points[i].x;
        ret.y += b[i] * points[i].y;
    }

    return ret;
}

void draw(SDL_Renderer *rend, SDL_Point *points)
{
    SDL_FPoint buf = bspline(points, 0.f);

    SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
    for (float i = 0.f; i < 1.f; i += .001f)
    {
        SDL_FPoint p1 = bspline(points, i);
        SDL_RenderDrawLine(rend, p1.x, p1.y, buf.x, buf.y);
        buf = p1;
    }
}

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *w = SDL_CreateWindow("Bezier curves", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 800, SDL_WINDOW_SHOWN);
    SDL_Renderer *rend = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event evt;

    SDL_Point points[] = {
        (SDL_Point){ 100, 400 },
        (SDL_Point){ 100, 400 },
        (SDL_Point){ 100, 400 },
        (SDL_Point){ 200, 250 },
        (SDL_Point){ 250, 600 },
        (SDL_Point){ 350, 300 },
        (SDL_Point){ 400, 500 },
        (SDL_Point){ 450, 200 },
        (SDL_Point){ 500, 600 },
        (SDL_Point){ 500, 600 },
        (SDL_Point){ 500, 600 }
    };

    size_t len = sizeof(points) / sizeof(SDL_Point);

    while (running)
    {
        while (SDL_PollEvent(&evt))
        {
            switch (evt.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            }
        }

        SDL_RenderClear(rend);

        for (size_t i = 0; i < len - 3; ++i)
            draw(rend, &points[i]);

        SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
        for (int i = 0; i < len - 1; ++i)
        {
            SDL_RenderDrawLine(rend, points[i].x, points[i].y, points[i + 1].x, points[i + 1].y);
        }

        SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
        SDL_RenderPresent(rend);
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(w);

    SDL_Quit();

    return 0;
}


/*

Copyright (c) 2023 Baptiste Leroyer

Permission is hereby granted, free of charge, to any
person obtaining a copy of this software and associated
documentation files (the "Software"), to deal in the
Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice
shall be included in all copies or substantial portions
of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS
OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#ifndef PXEG_H_
#define PXEG_H_

/***************
***** PXEG *****
  Pixel Engine
****************
Baptiste Leroyer
****************

PXEG is written in C using SDL2. It's meant
to be extremely simple, lightweight, and easy to use. By
initializing PXEG you instantly get a window with a ready
to use pixel perfect render context, handing you the control
over every pixel that is rendered on the screen.
The only external dependency needed is SDL2.

************** OS compliation flags ***************

MacOS:      -lSDL2
Linux:      -lSDL2
Windows:    -lSDL2

*************** Hello World Example ****************


#include "PXEG.h"

int main(int argc, char *argv[])
{
    PXEG_Window window;
    PXEG_Buffers *buffers = PXEG_init("Hello World", &window, 640, 480, 40, 40);

    PXEG_Update(&window, buffers);

    SDL_Delay(5000);

    PXEG_destroy(&window, buffers);

    return 0;
}

****************************************************/

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <stdbool.h>

#define PXGE_COLOR_BLACK    (PXEG_Color){0  , 0  , 0  , 255}
#define PXGE_COLOR_WHITE    (PXEG_Color){255, 255, 255, 255}
#define PXGE_COLOR_RED      (PXEG_Color){255, 0  , 0  , 255}
#define PXGE_COLOR_GREEN    (PXEG_Color){0  , 255, 0  , 255}
#define PXGE_COLOR_BLUE     (PXEG_Color){0  , 0  , 255, 255}
#define PXGE_COLOR_YELLOW   (PXEG_Color){255, 255, 0  , 255}
#define PXGE_COLOR_PURPLE   (PXEG_Color){255, 0  , 255, 255}
#define PXGE_COLOR_CYAN     (PXEG_Color){0  , 255, 255, 255}
#define PXGE_COLOR_ORANGE   (PXEG_Color){255, 165, 0  , 255}
#define PXGE_COLOR_BROWN    (PXEG_Color){165, 42 , 42 , 255}
#define PXGE_COLOR_PINK     (PXEG_Color){255, 192, 203, 255}
#define PXGE_COLOR_GREY     (PXEG_Color){128, 128, 128, 255}

/**
 * @brief structure of the window
 *
 * @param width Width of the window
 * @param height Height of the window
 * @param pixel_size Size of a pixel in pixels
 * @param window The SDL window
 * @param renderer The SDL renderer
 */
typedef struct PXEG_Window_t {
    int width;   // The width of the window
    int height;  // The height of the window
    int pixel_size;  // The size of a pixel in pixels
    int pixel_width;  // The number of pixels in width
    int pixel_height;  // The number of pixels in height
    SDL_Window* window;  // The SDL window
    SDL_Renderer* renderer;  // The SDL renderer
} PXEG_Window;

/**
 * @brief Struture of a color
 *
 * @param r The red component
 * @param g The green component
 * @param b The blue component
 * @param a The alpha component
 */
typedef struct PXEG_Color_t {
    int r; // The red component
    int g; // The green component
    int b; // The blue component
    int a; // The alpha component
} PXEG_Color;

/**
 * @brief Strcuture of the buffers
 *
 * @param pixels the pixel tab
 * @param z_buffer the z buffer tab
 */
typedef struct PXEG_Buffers_t {
    PXEG_Color **pixels; // The pixel tab
    float **z_buffer; // The z buffer tab
} PXEG_Buffers;

/**
 * @brief Initialize the window and return the buffer
 *
 * @param title the title of the window
 * @param window the window
 * @param width The width of the window
 * @param height The height of the window
 * @param pixel_width The number of pixels in width
 * @param pixel_height The number of pixels in height
 * @return PXEG_Buffers*
 */
PXEG_Buffers *PXEG_init(const char *title, PXEG_Window* window, int width, int height, int pixel_width, int pixel_height)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return NULL;
    }

    window->width = width;
    window->height = height;
    window->pixel_width = pixel_width;
    window->pixel_height = pixel_height;

    window->pixel_size = width / pixel_width;
    (height / pixel_height < window->pixel_size) ? (window->pixel_size = height / pixel_height) : (window->pixel_size = window->pixel_size);

    window->window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window->window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    window->renderer = SDL_CreateRenderer(window->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (window->renderer == NULL) {
        SDL_DestroyWindow(window->window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }

    PXEG_Buffers *buffers = malloc(sizeof(PXEG_Buffers));
    buffers->pixels = calloc(pixel_width, sizeof(PXEG_Color *));
    buffers->z_buffer = calloc(pixel_width, sizeof(float *));

    for (int i = 0; i < pixel_width; i++) {
        buffers->pixels[i] = calloc(pixel_height, sizeof(PXEG_Color));
        buffers->z_buffer[i] = calloc(pixel_height, sizeof(float));
    }

    return buffers;
}

/**
 * @brief Destroy the window and the buffer
 *
 * @param window The window to destroy
 * @param buffers The buffer to destroy
 */
void PXEG_destroy(PXEG_Window* window, PXEG_Buffers *buffers)
{
    for (int i = 0; i < window->pixel_width; i++) {
        free(buffers->pixels[i]);
        free(buffers->z_buffer[i]);
    }

    free(buffers->pixels);
    free(buffers->z_buffer);
    free(buffers);

    SDL_DestroyRenderer(window->renderer);
    SDL_DestroyWindow(window->window);
    SDL_Quit();
}

/**
 * @brief Update the window
 *
 * @param window the window
 * @param buffers the buffer
 */
void PXEG_Update(PXEG_Window* window, PXEG_Buffers *buffers)
{
    SDL_SetRenderDrawColor(window->renderer, 0, 0, 0, 255);
    SDL_RenderClear(window->renderer);

    for (int i = 0; i < window->pixel_width; i++) {
        for (int j = 0; j < window->pixel_height; j++) {
            SDL_Rect rect = {i * window->pixel_size, j * window->pixel_size, window->pixel_size, window->pixel_size};
            SDL_SetRenderDrawColor(window->renderer, buffers->pixels[i][j].r, buffers->pixels[i][j].g, buffers->pixels[i][j].b, buffers->pixels[i][j].a);
            SDL_RenderFillRect(window->renderer, &rect);
        }
    }

    SDL_RenderPresent(window->renderer);
}

/**
 * @brief Clear the buffer
 *
 * @param window the window
 * @param buffers the buffer
 */
void PXEG_Clear(PXEG_Window* window, PXEG_Buffers *buffers, PXEG_Color color)
{
    for (int i = 0; i < window->pixel_width; i++) {
        for (int j = 0; j < window->pixel_height; j++) {
            buffers->pixels[i][j].r = color.r;
            buffers->pixels[i][j].g = color.g;
            buffers->pixels[i][j].b = color.b;
            buffers->pixels[i][j].a = color.a;
        }
    }
}

/**
 * @brief Return if the window is open
 *
 * @return true if the window is open
 * @return false if the window is closed
 */
bool PXEG_Run()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Return the mouse position
 *
 * @param window the window
 * @param x The x position
 * @param y The y position
 */
void PXEG_Mouse_position(PXEG_Window* window, int *x, int *y)
{
    SDL_GetMouseState(x, y);
    *x = *x / window->pixel_size;
    *y = *y / window->pixel_size;
}

/**
 * @brief Return if the key is pressed
 *
 * @param key The key to try
 * @return true if the key is pressed
 * @return false if the key is not pressed
 */
bool PXEG_IsKeyPressed(SDL_Scancode key)
{
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    return state[key];
}

/**
 * @brief Return if the mouse button is pressed
 *
 * @param button The button to try
 * @return true if the button is pressed
 * @return false if the button is not pressed
 */
bool PXEG_IsMousePressed(int button)
{
    return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(button);
}

/**
 * @brief Return the color of a pixel
 *
 * @param buffers The buffer
 * @param x The x position of the pixel
 * @param y The y position of the pixel
 * @return PXEG_Color The color of the pixel
 */
PXEG_Color PXEG_GetPixel(PXEG_Buffers *buffers, int x, int y)
{
    return buffers->pixels[x][y];
}

/**
 * @brief Change the color of a pixel
 *
 * @param window The window
 * @param buffers The buffer
 * @param x The x position of the pixel
 * @param y The y position of the pixel
 * @param color The new color of the pixel
 */
void PXEG_SetPixel(PXEG_Window *window, PXEG_Buffers *buffers, int x, int y, PXEG_Color color)
{
    (x < window->pixel_width && y < window->pixel_height) ? buffers->pixels[x][y].r = color.r : 0;
    (x < window->pixel_width && y < window->pixel_height) ? buffers->pixels[x][y].g = color.g : 0;
    (x < window->pixel_width && y < window->pixel_height) ? buffers->pixels[x][y].b = color.b : 0;
    (x < window->pixel_width && y < window->pixel_height) ? buffers->pixels[x][y].a = color.a : 0;
}

/**
 * @brief Return the time since the call of PXEG_Init
 *
 * @return double The time in milliseconds
 */
double PXEG_Time(void)
{
    return SDL_GetTicks();
}

#endif /* !PXEG_H_ */

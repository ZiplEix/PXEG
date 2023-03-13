# PXEG

Pixel engine for C and C++

## What is PXEG ?

PXEG is a pixel engine for C and C++ base on the SDL2. It is a simple library that allows you to draw pixels on the screen. It is very easy to use, lightweight and fast.

## How to use it ?

PXEG is very easy to use. You just have to include the header file. Then you can use the functions to draw pixels on the screen.

A wiki is available in the wiki tab of the github repo

## How to compile ?

PXEG is a header only library. You just have to include the header file. Be careful, you have to link the SDL2 library to your project :

```bash
gcc -o my_program my_program.c -lSDL2
```

## How to contribute ?

You can contribute to PXEG by forking the project and making a pull request. You can also open an issue if you have a problem or a suggestion.

## Example

this is the simplest example of PXEG :

```c
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
```

You can find more examples in the examples folder.

## API

### Window

```c
typedef struct PXEG_Window_t {
    int width;   // The width of the window
    int height;  // The height of the window
    int pixel_size;  // The size of a pixel in pixels
    int pixel_width;  // The number of pixels in width
    int pixel_height;  // The number of pixels in height
    SDL_Window* window;  // The SDL window
    SDL_Renderer* renderer;  // The SDL renderer
} PXEG_Window;
```

> The window structure contains all the information about the window. You don't have to use it directly nor modify it.

### Color / Pixels

```c
typedef struct PXEG_Color_t {
    int r; // The red component
    int g; // The green component
    int b; // The blue component
    int a; // The alpha component
} PXEG_Color;
```

> The color structure contains all the information about a color. This structure represent a pixel

### Buffers

```c
typedef struct PXEG_Buffers_t {
    PXEG_Color **pixels; // The pixel tab
    float **z_buffer; // The z buffer tab
} PXEG_Buffers;
```

> The buffers structure contains all the information about the buffers. You don't have to use it directly nor modify it.

### Functions

```c
PXEG_Buffers *PXEG_init(const char *title, PXEG_Window* window, int width, int height, int pixel_width, int pixel_height)
```

> This function initialize the window and the buffers. It returns a pointer to the buffers. You have to pass the title of the window, the window structure, the width and height of the window in pixels, the width and height of the buffers of pixels.

```c
void PXEG_destroy(PXEG_Window* window, PXEG_Buffers *buffers)
```

> This function destroy the window and the buffers.

```c
void PXEG_Update(PXEG_Window* window, PXEG_Buffers *buffers)
```

> This function update the window with the buffers each time you call it.

```c
bool PXEG_Run()
```

> This function return true if the window is open and false if it is closed.

```c
void PXEG_Mouse_position(PXEG_Window* window, int *x, int *y)
```

> This function set the position of the mouse in the window in pixel (pixels of the buffer not of the window).

```c
bool PXEG_IsKeyPressed(SDL_Scancode key)
```

> This function return true if the key is pressed and false if it is not.

```c
bool PXEG_IsMousePressed(int button)
```

> This function return true if the mouse button is pressed and false if it is not.

```c
PXEG_Color PXEG_GetPixel(PXEG_Buffers *buffers, int x, int y)
```

> This function return the color of the pixel at the position (x, y) in the buffers.

```c
void PXEG_SetPixel(PXEG_Buffers *buffers, int x, int y, PXEG_Color color)
```

> This function set the color of the pixel at the position (x, y) in the buffers.

```c
double PXEG_Time(void)
```

> This function return the time in millieseconds since the call of ``PXEG_Init``

## Author

PXEG was created by [Baptiste Leroyer](https://github.com/ZiplEix).

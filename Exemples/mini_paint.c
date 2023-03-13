#include "../PXEG.h"

int main(int argc, char *argv[])
{
    PXEG_Window window;
    PXEG_Buffers *buffers = PXEG_init("Pixel Engine", &window, 800, 600, 80, 60);


    while (PXEG_Run()) {
        int x, y;
        PXEG_Mouse_position(&window, &x, &y);

        if (PXEG_IsKeyPressed(SDL_SCANCODE_Q)) {
            printf("Q pressed\n");
            break;
        }
        if (PXEG_IsKeyPressed(SDL_SCANCODE_H)) {
            printf("x: %d, y: %d\n", x, y);
        }

        if (PXEG_IsMousePressed(SDL_BUTTON_LEFT)) {
            PXEG_SetPixel(&window, buffers, x, y, (PXEG_Color){255, 0, 0, 255});
        }

        PXEG_Update(&window, buffers);
    }

    PXEG_destroy(&window, buffers);

    return 0;
}

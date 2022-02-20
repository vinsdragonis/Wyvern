#include "graphics/graphics.h"

int start() {
    // String literals are limited to 61 characters
    char header[] = "Welcome to Wyvern OS!";
    char *p = header;

    base = (unsigned int) &isr1;
    base12 = (unsigned int) &isr12;

    InitializeIDT();
    InitializeMouse();

    while (1) {
        clearScreen(181.0f / 255.0f * 16.0f, 232.0f / 255.0f * 32.0f, 255.0f / 255.0f * 16.0f);

        DrawRect(x, y, 10, 10, 0, 0, 0);

        DrawRect(150, 140, 350, 40, 0, 0, 0);
        DrawString(getArialCharacter, font_arial_width, font_arial_height, p, 240, 150, 255, 255, 255);

        DrawRect(300, 300, 50, 50, 20, 200, 50);
        DrawRect(325, 325, 50, 50, 200, 50, 50);

        Flush();
    }
}
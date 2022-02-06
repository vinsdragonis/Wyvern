#include "graphics.h"

int start() {
    clearScreen(181.0f / 255.0f * 16.0f, 232.0f / 255.0f * 32.0f, 255.0f / 255.0f * 16.0f);

    // String literals are limited to 61 characters
    char header[] = "Welcome to Wyvern OS!\n- Developed by Vineeth Dragonis";
    char *p = header;

    DrawString(getArialCharacter, font_arial_width, font_arial_height, p, 50, 50, 0, 0, 0);

    while (1);
}
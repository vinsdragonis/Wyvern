#include "graphics/graphics.h"

int start() {
    VBEInfoBlock *VBE = (VBEInfoBlock*) VBEInfoAddress;

    x = VBE->x_resolution / 2;
    y = VBE->y_resolution / 2;

    // String literals are limited to 61 characters
    char header[] = "Welcome to Wyvern OS!";
    char *p = header;

    char characterBuffer[1000] = "\0";
    char *characterBufferPointer = characterBuffer;
    int characterBufferLength = 0;

    base = (unsigned int) &isr1;
    base12 = (unsigned int) &isr12;

    InitializeIDT();
    InitializeMouse();

    while (1) {
        char c = ProcessScancode(Scancode);

        if (backspace_pressed == TRUE)
        {
            characterBuffer[characterBufferLength - 1] = '\0';
            characterBufferLength--;
            backspace_pressed = FALSE;
            Scancode = -1;
        }
        else if (c != '\0')
        {
            characterBuffer[characterBufferLength] = c;
            characterBuffer[characterBufferLength + 1] = '\0';
            characterBufferLength++;
            Scancode = -1;
        }

        clearScreen(90.0f / 255.0f * 16.0f, 120.0f / 255.0f * 32.0f, 120.0f / 255.0f * 16.0f);

        DrawRect(150, 140, 350, 40, 0, 0, 0);
        DrawString(getArialCharacter, font_arial_width, font_arial_height, p, 240, 150, 255, 255, 255);
        DrawString(getArialCharacter, font_arial_width, font_arial_height, characterBufferPointer, 240, 180, 255, 255, 255);

        DrawRect(300, 300, 50, 50, 20, 200, 50);
        DrawRect(325, 325, 50, 50, 200, 50, 50);

        // DrawRect(x, y, 10, 10, 0, 0, 0);
        DrawMouse(x, y, 50.0, 150.0 / 255.0 * 32, 150.0 / 255.0 * 16);

        Flush();
    }
}
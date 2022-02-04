#include "graphics.h"

// R = 4 bits
// G = 5 bits
// B = 4 bits

int rgb(int r, int g, int b)
{
    r = (int)(r / 3);
    g = (int)(g / 2);
    b = (int)(b / 3);

    return r << 11 | g << 5 | b;
}

void Draw(int x, int y, int r, int g, int b) {
    VBEInfoBlock* VBE = (VBEInfoBlock*) VBEInfoAddress;
    int index = y * VBE->x_resolution + x;
    *((unsigned short*) VBE->screen_ptr + index) = rgb(r, g, b);
}

void clearScreen(int r, int g, int b) {
    VBEInfoBlock* VBE = (VBEInfoBlock*) VBEInfoAddress;
    
    for (int y = 0; y < VBE->y_resolution; y++) {
        for (int x = 0; x < VBE->x_resolution; x++) {
            Draw(x, y, r, g, b);
        }
    }
    
}

void drawRect(int x, int y, int width, int height, int r, int g, int b) {
    for (int j = y; j < (y + height); j++) {
        for (int i = x; i < (x + width); i++) {
            Draw(i, j, r, g, b);
        }
    }
}
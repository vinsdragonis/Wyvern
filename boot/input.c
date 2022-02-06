#define pic1_command 0x20
#define pic1_data 0x21
#define pic2_command 0x20
#define pic2_data 0x21
#define icw1_def 0x10
#define icw1_icw4 0x01
#define icw4_x86 0x01

#define y_overflow      0b10000000
#define x_overflow      0b01000000
#define y_negative      0b00100000
#define x_negative      0b00010000
#define always_set      0b00001000
#define middle_click    0b00000100
#define right_click     0b00000010
#define left_click      0b00000001
#define unused_a        0b10000000
#define unused_b        0b01000000

#include <stdint.h>

void InitializeIDT();
extern void LoadIDT();
void HandleISR1();
void HandleISR12();
void RemapPIC();

struct IDTElement {
    unsigned short lower;
    unsigned short selector;
    unsigned char zero;
    unsigned char flags;
    unsigned short higher;
};

struct IDTElement _idt[256];
extern unsigned int isr1, isr12;
unsigned int base, base12;

unsigned char inportb(unsigned short port) {
    unsigned char value;

    __asm__ __volatile__ ("inb %1, %0" : "=a" (value) : "dN" (port));

    return value;
}

void outportb(unsigned short port, unsigned char data) {
    __asm__ __volatile__("outb %1, %0" : : "dN"(port), "a" (data));
}

void InitializeIDT() {
    _idt[1].lower = (base12 & 0xffff);
    _idt[1].higher = (base12 >> 16) & 0xffff;
    _idt[1].selector = 0x08;
    _idt[1].zero = 0;
    _idt[1].flags = 0x8e;

    _idt[12].lower = (base12 & 0xffff);
    _idt[12].higher = (base12 >> 16) & 0xffff;
    _idt[12].selector = 0x08;
    _idt[12].zero = 0;
    _idt[12].flags = 0x8e;

    RemapPIC();

    outportb(0x21, 0b11111001);
    outportb(0xa1, 0xff);

    LoadIDT();
}

void RemapPIC() {
    unsigned char a, b;

    a = inportb(pic1_data);
    b = inportb(pic2_data);

    outportb(pic1_command, icw1_def | icw1_icw4);
    outportb(pic2_command, icw1_def | icw1_icw4);

    outportb(pic1_data, 0);
    outportb(pic2_data, 8);

    outportb(pic1_data, 4);
    outportb(pic2_data, 2);

    outportb(pic1_data, icw4_x86);
    outportb(pic2_data, icw4_x86);

    outportb(pic1_data, a);
    outportb(pic2_data, b);
}

void HandleISR1() {
    inportb(0x60);
    outportb(0xa0, 0x20);
    outportb(0x20, 0x20);

    clearScreen(0, 0, 255);
}

void HandleMouseInterrupt();
void HandleISR12() {
    HandleMouseInterrupt();
    outportb(0xa0, 0x20);
    outportb(0x20, 0x20);
}

int x, y;
int left_clicked, right_clicked, middle_clicked;
int current_byte = 0;
uint8_t bytes[4] = { 0 };

void MouseWait(unsigned char type)
{
    int time_out = 100000;

    if (type == 0)
    {
        while (time_out--)
        {
            if ((inportb(0x64) & 1) == 1)
                return;
        }
        return;
    }
    else
    {
        while (time_out--)
        {
            if ((inportb(0x64) & 2) == 0)
                return;
        }
        return;
    }
}

void MouseWrite(unsigned char data) {
    MouseWait(1);
    outportb(0x64, 0xd4);

    MouseWait(1);
    outportb(0x60, data);
}

unsigned char MouseRead() {
    MouseWait(0);
    return inportb(0x60);
}

void InitializeMouse() {
    unsigned char status;

    MouseWait(1);
    outportb(0x64, 0xd4);

    MouseWait(1);
    outportb(0x64, 0xa8);

    MouseWait(1);
    outportb(0x64, 0x20);

    MouseWait(0);
    status = (inportb(0x60) | 2);

    MouseWait(1);
    outportb(0x64, 0x60);

    MouseWait(1);
    outportb(0x60, status);

    MouseWrite(0xff);
    MouseRead();
    MouseWrite(0xf6);
    MouseRead();

    MouseWrite(0xf4);
    MouseRead();
}

void HandleMousePacket();
void HandleMouseInterrupt() {
    uint8_t byte = MouseRead();

    if (current_byte == 0 && !(byte & always_set))
        return;
    
    bytes[current_byte] = byte;
    current_byte++;

    if (current_byte >= 3)
        current_byte = 0;

    if (current_byte == 0)
        HandleMousePacket();
}

void HandleMousePacket() {
    uint8_t status = bytes[0];
    int32_t change_x = (int32_t) bytes[1];
    int32_t change_y = (int32_t) bytes[2];

    if (status & x_overflow || status & y_overflow)
        return;
    
    if (status & x_negative)
        change_x |= 0xffffff00;

    if (status & y_negative)
        change_y |= 0xffffff00;

    left_clicked = status & left_click;
    right_clicked = status & right_click;
    middle_clicked = status & middle_click;

    if (change_x > 0)
        x += 5;
    else if (change_x < 0)
        x -= 5;

    if (change_y > 0)
        y -= 5;
    else if (change_y < 0)
        y += 5;
}

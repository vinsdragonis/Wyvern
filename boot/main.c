char* video_memory;

int start() {
    video_memory = (char*) 0xb8000;

    for (int i = 0; i < 2 * 25 * 80; i += 2) {
        *(video_memory + i) = 0;
        *(video_memory + i + 1) = 0x0B;
    }
}
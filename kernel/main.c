int print_vga(const char* str, int line) {
    char* vgaMemory = (char*)(0xb8000 + (160 * line));
    int counter = 0;
    while (*str) {
        *vgaMemory = *str;
        *(vgaMemory + 1) = 0xe;
        vgaMemory += 2;
        str += 1;
        counter += 1;
    }
    while (counter % 80 != 0) {
        *(vgaMemory + 1) = 0x0;
        vgaMemory += 2;
        counter += 1;
    }
    return 1 + (counter / 80);
}

int main() {
    print_vga("Kernel reached! :)", 0);
    return 0;
}
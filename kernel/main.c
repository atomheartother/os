int print_vga(const char* str, int line, char color) {
    char* vgaMemory = (char*)(0xb8000 + (160 * line));
    int counter = 0;
    while (*str) {
        *vgaMemory = *str;
        *(vgaMemory + 1) = color;
        vgaMemory += 2;
        str += 1;
        counter += 1;
    }
    while (counter % 80 != 0) {
        *(vgaMemory) = ' ';
        *(vgaMemory + 1) = color;
        vgaMemory += 2;
        counter += 1;
    }
    return (counter / 80);
}

int main() {
    print_vga("Kernel reached! :)", 0, 0xe);
    print_vga("", 1, 0x0);
    print_vga("No matter where you go,", 2, 0x1e);
    print_vga("    everyone is connected", 3, 0x1e);
    return 0;
}
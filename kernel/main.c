
int main() {
    char* vgaMemory = (char*)0xb8000;
    *vgaMemory = 'X';
    return 0;
}
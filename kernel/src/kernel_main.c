void _start(void)
{
    char* vga = (char*)0xb8000;
    *vga = 0x67;

    return;
}

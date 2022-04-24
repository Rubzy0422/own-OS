
#include <types.h>
#include <gdt.h>

static uint16_t* VideoMemory = (uint16_t*)0xb8000;
static uint8_t x = 0, y =0;


void clear() {
 for (y = 0; y < 25; y++)
                for (x = 0; x < 80; x++)
                    VideoMemory[80 * (y)+(x)] = (VideoMemory[80 * (y)+(x)] & 0xFF00) | ' ';
            x = 0;
            y = 0;
}

void printf(char *str) {
    // 80 * 25
    for (int i = 0; str[i] != '\0'; ++i) {

        switch (str[i])
        {
            case '\n':
                y++;
                x = 0;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        
        // Column
        if (x >= 80)
        {
            y++;
            x = 0;
        }
        // row 
        if (y >= 25)
        {
         clear();
        }
    }
}

extern "C" void kernelMain(const void * multiboot_structure, uint32_t) {
    clear();
    printf("Hello World!\n");

    GlobalDescriptorTable gdt;
    while(1)
    return;
}
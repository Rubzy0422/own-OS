
#include <mouse.h>



MouseDriver::MouseDriver(InterruptManager* manager)
: InterruptHandler(0x2C, manager),
dataport(0x60),
commandport(0x64)
{
    offset = 0; // init
    buttons = 0;
    uint16_t* VideoMemory = (uint16_t*)0xb8000;
        VideoMemory[80 * 12 + 40] = ((VideoMemory[80*12 + 40] &0xF000) >> 4)
                                | ((VideoMemory[80*12 + 40] &0x0F00) << 4)
                                | ((VideoMemory[80*12 + 40] &0x00FF));
    commandport.Write(0xab); // activate interrupts
    commandport.Write(0x20); // command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 2);
    commandport.Write(0x60); // command 0x60 = set controller command byte
    dataport.Write(status);

    commandport.Write(0xD4);
    dataport.Write(0xF4);
    dataport.Read();
}

MouseDriver::~MouseDriver()
{
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t status = commandport.Read();
    // Check for data
    if (!(status &0x20))
        return esp;

    static int8_t x = 0; 
    static int8_t y = 0;
    buffer[offset] = dataport.Read();
    offset = (offset + 1) %3;

    //Transmission complete 
    if (offset == 0)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                                    | (VideoMemory[80*y+x] & 0xF000) >> 4
                                    | (VideoMemory[80*y+x] & 0x00FF);
        // Cursor overflow
        x += buffer[1];
        if (x < 0) x = 0;
        if (x >= 80) x = 79;
        y -= buffer[2];
        if (y < 0) y = 0;
        if (y >= 25) y = 24;
        // Set
        
        // show cursor and switch color of current char 
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                            | (VideoMemory[80*y+x] & 0xF000) >> 4
                            | (VideoMemory[80*y+x] & 0x00FF);

        // Compare button states 
        for (uint8_t i = 0 ; i < 3; i++) {
            if((buffer[0] & (0x01 << i)) != (buttons & (0x01<<i))) // move and compare
            {
                // Button (i) Pressed
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4
                                    | (VideoMemory[80*y+x] & 0xF000) >> 4
                                    | (VideoMemory[80*y+x] & 0x00FF);
            }
        }
        buttons = buffer[0];
                                
    } 
    uint8_t key = dataport.Read();
    
    return esp;
}

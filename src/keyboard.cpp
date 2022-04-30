#include <keyboard.h>

KeyboardDriver::KeyboardDriver(InterruptManager* manager)
: InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
    // Hold key
    while (commandport.Read() & 0x1)
    {
        dataport.Read();
    }
    commandport.Write(0xAE); //activate intterrupts
    commandport.Write(0x20); // get curent state
    uint8_t status = (dataport.Read() | 1) & -0x10;//set rightmost but to one and clear 5th bit
    commandport.Write(0x60); //set state
    dataport.Write(status); 

    dataport.Write(0xF4); //activate keyboard
}

KeyboardDriver::~KeyboardDriver()
{
}

void printf(char *);

uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp) {
    
    // key strike 
    uint8_t key = dataport.Read();

    if (key < 0x80) // after is key release 
    {
        switch (key)
        {
            case 0xFA: break;
            case 0x45: case 0xC5: break;

            default:
                char* foo = "KEYBOARD 0x00 ";
                char* hex = "0123456789ABCDEF";

                foo[11] = hex[(key >> 4) & 0xF];
                foo[12] = hex[key & 0xF];
                printf(foo);
                break;    
        }
        return esp;
    }
}


#include <keyboard.h>



KeyboardDriver::KeyboardDriver(InterruptManager* manager)
: InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
    while(commandport.Read() & 0x1)
        dataport.Read();
    commandport.Write(0xae); // activate interrupts
    commandport.Write(0x20); // command 0x20 = read controller command byte
    uint8_t status = (dataport.Read() | 1) & ~0x10;
    commandport.Write(0x60); // command 0x60 = set controller command byte
    dataport.Write(status);
    dataport.Write(0xf4);
}

KeyboardDriver::~KeyboardDriver()
{
}

void printf(char*);


uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();
       
        switch(key)
        {
            static bool shift = false;
            static bool capslock = false;
            
            case 0x01: break; //Escape 
            case 0x3B: break; //F1
            case 0x3C: break; //F2
            case 0x3D: break; //F3
            case 0x3E: break; //F4
            case 0x3F: break; //F5
            case 0x40: break; //F6
            case 0x41: break; //F7
            case 0x42: break; //F8
            case 0x43: break; //F9
            case 0x44: break; //F10

            case 0x57: break; //F11
            case 0x58: break; //F12

            case 0x1D: break; //LFT_CTRL
            case 0x38: break; //LFT ALT

            case 0x0E: break; //backspace
            
            
            
            
            // Press 
            case 0X3A: capslock = !capslock; break; //capslock

            case 0x5B: break; //windows key
            case 0x5D: break; //menu key 

            case 0x37: break;//prtscrn
            case 0x45: break; // pause 
            case 0x52: break; //insert
            case 0x47: break; //home
            case 0x49: break; //pg up 
            case 0x53: break; //delete
            case 0x4f: break; // end
            case 0x51: break; //pg down

            case 0x48: break;//up
            case 0x50: break;//down
            case 0x4B: break;//left
            case 0x4D: break;//right

            // case 0x37: printf("*");
            case 0x4A: printf("-");
            case 0x4E: printf("+");
            case 0x4C: printf("5");



            case 0x29: shift ? printf("~") : printf("`"); break;
            case 0x02: shift ? printf("!") : printf("1"); break;
            case 0x03: shift ? printf("@") : printf("2"); break;
            case 0x04: shift ? printf("#") : printf("3"); break;
            case 0x05: shift ? printf("$") : printf("4"); break;
            case 0x06: shift ? printf("%") : printf("5"); break;
            case 0x07: shift ? printf("^") : printf("6"); break;
            case 0x08: shift ? printf("&") : printf("7"); break;
            case 0x09: shift ? printf("*") : printf("8"); break;
            case 0x0A: shift ? printf("(") : printf("9"); break;
            case 0x0B: shift ? printf(")") : printf("0"); break;
            case 0x0C: shift ? printf("_") : printf("-"); break;
            case 0x0D: shift ? printf("+") : printf("+"); break;

            case 0x10: shift ^ capslock ? printf("Q") : printf("q"); break;
            case 0x11: shift ^ capslock ? printf("W") : printf("w"); break;
            case 0x12: shift ^ capslock ? printf("E") : printf("e"); break;
            case 0x13: shift ^ capslock ? printf("R") : printf("r"); break;
            case 0x14: shift ^ capslock ? printf("T") : printf("t"); break;
            case 0x15: shift ^ capslock ? printf("Y") : printf("y"); break;
            case 0x16: shift ^ capslock ? printf("U") : printf("u"); break;
            case 0x17: shift ^ capslock ? printf("I") : printf("i"); break;
            case 0x18: shift ^ capslock ? printf("O") : printf("o"); break;
            case 0x19: shift ^ capslock ? printf("P") : printf("p"); break;
            case 0x1A: shift ? printf("{") : printf("["); break;
            case 0x1B: shift ? printf("}") : printf("]"); break;
            case 0x2B: shift ? printf("|") : printf("\\"); break;
            
            case 0x1E: shift ^ capslock ? printf("A") : printf("a"); break;
            case 0x1F: shift ^ capslock ? printf("S") : printf("s"); break;
            case 0x20: shift ^ capslock ? printf("D") : printf("d"); break;
            case 0x21: shift ^ capslock ? printf("F") : printf("f"); break;
            case 0x22: shift ^ capslock ? printf("G") : printf("g"); break;
            case 0x23: shift ^ capslock ? printf("H") : printf("h"); break;
            case 0x24: shift ^ capslock ? printf("J") : printf("j"); break;
            case 0x25: shift ^ capslock ? printf("K") : printf("k"); break;
            case 0x26: shift ^ capslock ? printf("L") : printf("l"); break;
            case 0x27: shift ? printf(":") : printf(";"); break;
            case 0x28: shift ? printf("\"") : printf("'"); break;

            case 0x2C: shift ^ capslock ? printf("Z") : printf("z"); break;
            case 0x2D: shift ^ capslock ? printf("X") : printf("x"); break;
            case 0x2E: shift ^ capslock ? printf("C") : printf("c"); break;
            case 0x2F: shift ^ capslock ? printf("V") : printf("v"); break;
            case 0x30: shift ^ capslock ? printf("B") : printf("b"); break;
            case 0x31: shift ^ capslock ? printf("N") : printf("n"); break;
            case 0x32: shift ^ capslock ? printf("M") : printf("m"); break;
            case 0x33: shift ? printf("<") : printf(","); break;
            case 0x34: shift ? printf(">") : printf("."); break;
            case 0x35: shift ? printf("?") : printf("/"); break;
            

            case 0x1C: printf("\n"); break;
            case 0x0f: printf("    "); break; // tab simulate 4 spaces
            case 0x39: printf(" "); break;

            // Press Shift
            case 0x2A: case 0x36: shift = true; break;
            // Release Shift
            case 0xAA: case 0xB6: shift = false; break;

            default:
            if(key < 0x80)
            {
                {
                    char* foo = "KEYBOARD 0x00\n";
                    char* hex = "0123456789ABCDEF";
                    foo[11] = hex[(key >> 4) & 0xF];
                    foo[12] = hex[key & 0xF];
                    printf(foo);
                    break;
                }
            }
    }
    return esp;
}


#include <keyboard.h>

void printf(char* value);
void printfHex(uint8_t key);


KeyboardEventHandler::KeyboardEventHandler(/* args */) {
}

KeyboardEventHandler::~KeyboardEventHandler() {
}

void KeyboardEventHandler::OnKeyDown(char) {

}

void KeyboardEventHandler::OnKeyUp(char) {

}


KeyboardDriver::KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler)
: InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
    this->handler = handler;
}

void KeyboardDriver::Activate() 
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


uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
    uint8_t key = dataport.Read();
    if (handler == 0)
        return esp; 

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
            case 0x4A: handler->OnKeyDown('-');
            case 0x4E: handler->OnKeyDown('+');
            case 0x4C: handler->OnKeyDown('5');



            case 0x29: shift ? handler->OnKeyDown('~') : handler->OnKeyDown('`'); break;
            case 0x02: shift ? handler->OnKeyDown('!') : handler->OnKeyDown('1'); break;
            case 0x03: shift ? handler->OnKeyDown('@') : handler->OnKeyDown('2'); break;
            case 0x04: shift ? handler->OnKeyDown('#') : handler->OnKeyDown('3'); break;
            case 0x05: shift ? handler->OnKeyDown('$') : handler->OnKeyDown('4'); break;
            case 0x06: shift ? handler->OnKeyDown('%') : handler->OnKeyDown('5'); break;
            case 0x07: shift ? handler->OnKeyDown('^') : handler->OnKeyDown('6'); break;
            case 0x08: shift ? handler->OnKeyDown('&') : handler->OnKeyDown('7'); break;
            case 0x09: shift ? handler->OnKeyDown('*') : handler->OnKeyDown('8'); break;
            case 0x0A: shift ? handler->OnKeyDown('(') : handler->OnKeyDown('9'); break;
            case 0x0B: shift ? handler->OnKeyDown(')') : handler->OnKeyDown('0'); break;
            case 0x0C: shift ? handler->OnKeyDown('_') : handler->OnKeyDown('-'); break;
            case 0x0D: shift ? handler->OnKeyDown('+') : handler->OnKeyDown('+'); break;

            case 0x10: shift ^ capslock ? handler->OnKeyDown('Q') : handler->OnKeyDown('q'); break;
            case 0x11: shift ^ capslock ? handler->OnKeyDown('W') : handler->OnKeyDown('w'); break;
            case 0x12: shift ^ capslock ? handler->OnKeyDown('E') : handler->OnKeyDown('e'); break;
            case 0x13: shift ^ capslock ? handler->OnKeyDown('R') : handler->OnKeyDown('r'); break;
            case 0x14: shift ^ capslock ? handler->OnKeyDown('T') : handler->OnKeyDown('t'); break;
            case 0x15: shift ^ capslock ? handler->OnKeyDown('Y') : handler->OnKeyDown('y'); break;
            case 0x16: shift ^ capslock ? handler->OnKeyDown('U') : handler->OnKeyDown('u'); break;
            case 0x17: shift ^ capslock ? handler->OnKeyDown('I') : handler->OnKeyDown('i'); break;
            case 0x18: shift ^ capslock ? handler->OnKeyDown('O') : handler->OnKeyDown('o'); break;
            case 0x19: shift ^ capslock ? handler->OnKeyDown('P') : handler->OnKeyDown('p'); break;
            case 0x1A: shift ? handler->OnKeyDown('{') : handler->OnKeyDown('['); break;
            case 0x1B: shift ? handler->OnKeyDown('}') : handler->OnKeyDown(']'); break;
            case 0x2B: shift ? handler->OnKeyDown('|') : handler->OnKeyDown('\\'); break;
            
            case 0x1E: shift ^ capslock ? handler->OnKeyDown('A') : handler->OnKeyDown('a'); break;
            case 0x1F: shift ^ capslock ? handler->OnKeyDown('S') : handler->OnKeyDown('s'); break;
            case 0x20: shift ^ capslock ? handler->OnKeyDown('D') : handler->OnKeyDown('d'); break;
            case 0x21: shift ^ capslock ? handler->OnKeyDown('F') : handler->OnKeyDown('f'); break;
            case 0x22: shift ^ capslock ? handler->OnKeyDown('G') : handler->OnKeyDown('g'); break;
            case 0x23: shift ^ capslock ? handler->OnKeyDown('H') : handler->OnKeyDown('h'); break;
            case 0x24: shift ^ capslock ? handler->OnKeyDown('J') : handler->OnKeyDown('j'); break;
            case 0x25: shift ^ capslock ? handler->OnKeyDown('K') : handler->OnKeyDown('k'); break;
            case 0x26: shift ^ capslock ? handler->OnKeyDown('L') : handler->OnKeyDown('l'); break;
            case 0x27: shift ? handler->OnKeyDown(':') : handler->OnKeyDown(';'); break;
            case 0x28: shift ? handler->OnKeyDown('\'') : handler->OnKeyDown('"'); break;

            case 0x2C: shift ^ capslock ? handler->OnKeyDown('Z') : handler->OnKeyDown('z'); break;
            case 0x2D: shift ^ capslock ? handler->OnKeyDown('X') : handler->OnKeyDown('x'); break;
            case 0x2E: shift ^ capslock ? handler->OnKeyDown('C') : handler->OnKeyDown('c'); break;
            case 0x2F: shift ^ capslock ? handler->OnKeyDown('V') : handler->OnKeyDown('v'); break;
            case 0x30: shift ^ capslock ? handler->OnKeyDown('B') : handler->OnKeyDown('b'); break;
            case 0x31: shift ^ capslock ? handler->OnKeyDown('N') : handler->OnKeyDown('n'); break;
            case 0x32: shift ^ capslock ? handler->OnKeyDown('M') : handler->OnKeyDown('m'); break;
            case 0x33: shift ? handler->OnKeyDown('<') : handler->OnKeyDown(','); break;
            case 0x34: shift ? handler->OnKeyDown('>') : handler->OnKeyDown('.'); break;
            case 0x35: shift ? handler->OnKeyDown('?') : handler->OnKeyDown('/'); break;
            

            case 0x1C: handler->OnKeyDown('\n'); break;
            // case 0x0f: handler->OnKeyDown('    '); break; // tab simulate 4 spaces
            case 0x39: handler->OnKeyDown(' '); break;

            // Press Shift
            case 0x2A: case 0x36: shift = true; break;
            // Release Shift
            case 0xAA: case 0xB6: shift = false; break;

            default:
            if(key < 0x80)
            {
               printf("KEYBOARD 0x");
               printfHex(key);
            }
    }
    return esp;
}

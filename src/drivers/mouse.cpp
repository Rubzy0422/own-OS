
#include <drivers/mouse.h>

using namespace kernelos::common;
using namespace kernelos::drivers;
using namespace kernelos::hardwarecommunication;

void printf(char* value);
void printfHex(uint8_t key);

// ABSTRACTION (does not contain the code for the event. Please reffer to the handler that it was built with) 
    MouseEventHandler::MouseEventHandler() {

    }

 // ABSTRACTION (does not contain the code for the event. Please reffer to the handler that it was built with) 
    void MouseEventHandler::OnActivate() {
    }

// ABSTRACTION (does not contain the code for the event. Please reffer to the handler that it was built with) 
    void MouseEventHandler::OnMouseDown(uint8_t btn) {

    }
// ABSTRACTION (does not contain the code for the event. Please reffer to the handler that it was built with) 
    void MouseEventHandler::OnMouseUp(uint8_t btn) {

    }

// ABSTRACTION (does not contain the code for the event. Please reffer to the handler that it was built with) 
    void MouseEventHandler::OnMouseMove(int x, int y) {

    }



    MouseDriver::MouseDriver(InterruptManager* manager, MouseEventHandler* handler)
    : InterruptHandler(0x2C, manager),
    dataport(0x60),
    commandport(0x64)
    {
        this->handler = handler;
    }

    void MouseDriver::Activate() {
        offset = 0;
        buttons = 0;
       
        if(handler != 0)
            handler->OnActivate();

        commandport.Write(0xA8);
        commandport.Write(0x20); // command 0x60 = read controller command byte
        uint8_t status = dataport.Read() | 2;
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
        if (!(status & 0x20))
            return esp;

        buffer[offset] = dataport.Read();
        
        if(handler == 0)
            return esp;
        
        offset = (offset + 1) % 3;

        if(offset == 0)
        {
            if(buffer[1] != 0 || buffer[2] != 0)
            {
                handler->OnMouseMove(buffer[1], -buffer[2]);
            }

            for(uint8_t i = 0; i < 3; i++)
            {
                if((buffer[0] & (0x1<<i)) != (buttons & (0x1<<i)))
                {
                    if(buttons & (0x1<<i))
                        handler->OnMouseUp(i+1);
                    else
                        handler->OnMouseDown(i+1);
                }
            }
            buttons = buffer[0];
        }
        return esp;
    }
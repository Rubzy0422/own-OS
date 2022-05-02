
#ifndef __DRIVERS_MOUSE_H
#define __DRIVERS_MOUSE_H


    #include <common/types.h>
    #include <hardwarecommunication/port.h>
    #include <drivers/driver.h>
    #include <hardwarecommunication/interrupts.h>

namespace kernelos {
    namespace drivers {
    
    class MouseEventHandler
    {
        public:
            MouseEventHandler();

            virtual void OnActivate();
            virtual void OnMouseDown(kernelos::common::uint8_t button);
            virtual void OnMouseUp(kernelos::common::uint8_t button);
            virtual void OnMouseMove(int x, int y);
        };


        class MouseDriver : public kernelos::hardwarecommunication::InterruptHandler, public Driver
        {
            kernelos::hardwarecommunication::Port8Bit dataport;
            kernelos::hardwarecommunication::Port8Bit commandport;
            kernelos::common::uint8_t buffer[3];
            kernelos::common::uint8_t offset;
            kernelos::common::uint8_t buttons;

            MouseEventHandler* handler;
        public:
            MouseDriver(kernelos::hardwarecommunication::InterruptManager* manager, MouseEventHandler* handler);
            ~MouseDriver();
            virtual kernelos::common::uint32_t HandleInterrupt(kernelos::common::uint32_t esp);
            virtual void Activate();
        };
    }
}
#endif

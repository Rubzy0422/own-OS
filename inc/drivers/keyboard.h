#ifndef __DRIVERS_KEYBOARD_H
#define __DRIVERS_KEYBOARD_H

    #include <common/types.h>
    #include <hardwarecommunication/interrupts.h>
    #include <drivers/driver.h>
    #include <hardwarecommunication/port.h>

namespace kernelos {
    namespace drivers {

        class KeyboardEventHandler
        {
            public:
                KeyboardEventHandler(/* args */);
                ~KeyboardEventHandler();

        // Move from char to enum
                virtual void OnKeyDown(char c);
                virtual void OnKeyUp(char c);
        };


            class KeyboardDriver : public kernelos::hardwarecommunication::InterruptHandler, public Driver
            {
                kernelos::hardwarecommunication::Port8Bit dataport;
                kernelos::hardwarecommunication::Port8Bit commandport;

                KeyboardEventHandler* handler;
            public:
                KeyboardDriver(kernelos::hardwarecommunication::InterruptManager* manager, KeyboardEventHandler *handler);
                ~KeyboardDriver();
                virtual kernelos::common::uint32_t HandleInterrupt(kernelos::common::uint32_t esp);
                virtual void Activate();
            };

    }
}
#endif
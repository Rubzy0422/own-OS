
#ifndef __INTERRUPTMANAGER_H
#define __INTERRUPTMANAGER_H

    #include <gdt.h>
    #include <types.h>
    #include <port.h>


    class InterruptManager;

    class InterruptHandler
    {
    protected:
        uint8_t interruptNumber;
        InterruptManager* interruptManager;
        InterruptHandler(uint8_t InterruptNumber, InterruptManager* interruptManager);
        ~InterruptHandler();
    public:
        virtual uint32_t HandleInterrupt(uint32_t esp);
    };
    

         class InterruptManager
    {
        friend class InterruptHandler;
        //friend class InterruptHandler;
        protected:

            static InterruptManager* ActiveInterruptManager;
            InterruptHandler * handlers[256];

            struct GateDescriptor
            {
                uint16_t handlerAddressLowBits;
                uint16_t gdt_codeSegmentSelector;
                uint8_t reserved;
                uint8_t access;
                uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                uint16_t size;
                uint32_t base;
            } __attribute__((packed));

            uint16_t hardwareInterruptOffset;
            //static InterruptManager* ActiveInterruptManager;
            static void SetInterruptDescriptorTableEntry(uint8_t interrupt,
                uint16_t codeSegmentSelectorOffset, void (*handler)(),
                uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType);


            
            Port8BitSlow PICMasterCommandPort;
            Port8BitSlow PICMasterDataPort;
            Port8BitSlow PICSlaveCommandPort;
            Port8BitSlow PICSlaveDataPort;

        public:
            InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable);
            ~InterruptManager();

            uint16_t HardwareInterruptOffset();

            void Activate();
            
            static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);
            uint32_t DoHandleInterrupt(uint8_t intterruptnumber, uint32_t esp);

            static void InterruptIgnore();
            static void HandleInterruptRequest0x00(); // handle ignore
            static void HandleInterruptRequest0x01(); // handle keyboard
            static void HandleInterruptRequest0x0C(); // handle mouse
          


            void Deactivate();
    };

   
#endif
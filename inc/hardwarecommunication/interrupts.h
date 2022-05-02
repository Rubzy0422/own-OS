
#ifndef __HARDWARECOMMUNICATION_INTERRUPTMANAGER_H
#define __HARDWARECOMMUNICATION_INTERRUPTMANAGER_H

    #include <gdt.h>
    #include <common/types.h>
    #include <hardwarecommunication/port.h>

namespace kernelos {
    namespace hardwarecommunication
    {
         class InterruptManager;

    class InterruptHandler
    {
    protected:
        kernelos::common::uint8_t interruptNumber;
        InterruptManager* interruptManager;
        InterruptHandler(kernelos::common::uint8_t InterruptNumber, InterruptManager* interruptManager);
        ~InterruptHandler();
    public:
        virtual kernelos::common::uint32_t HandleInterrupt(kernelos::common::uint32_t esp);
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
                kernelos::common::uint16_t handlerAddressLowBits;
                kernelos::common::uint16_t gdt_codeSegmentSelector;
                kernelos::common::uint8_t reserved;
                kernelos::common::uint8_t access;
                kernelos::common::uint16_t handlerAddressHighBits;
            } __attribute__((packed));

            static GateDescriptor interruptDescriptorTable[256];

            struct InterruptDescriptorTablePointer
            {
                kernelos::common::uint16_t size;
                kernelos::common::uint32_t base;
            } __attribute__((packed));

            kernelos::common::uint16_t hardwareInterruptOffset;
            //static InterruptManager* ActiveInterruptManager;
            static void SetInterruptDescriptorTableEntry(kernelos::common::uint8_t interrupt,
                kernelos::common::uint16_t codeSegmentSelectorOffset, void (*handler)(),
                kernelos::common::uint8_t DescriptorPrivilegeLevel, kernelos::common::uint8_t DescriptorType);


            
            Port8BitSlow PICMasterCommandPort;
            Port8BitSlow PICMasterDataPort;
            Port8BitSlow PICSlaveCommandPort;
            Port8BitSlow PICSlaveDataPort;

        public:
            InterruptManager(kernelos::common::uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable);
            ~InterruptManager();

            kernelos::common::uint16_t HardwareInterruptOffset();

            void Activate();
            
            static kernelos::common::uint32_t HandleInterrupt(kernelos::common::uint8_t interrupt, kernelos::common::uint32_t esp);
            kernelos::common::uint32_t DoHandleInterrupt(kernelos::common::uint8_t intterruptnumber, kernelos::common::uint32_t esp);

            static void InterruptIgnore();
            static void HandleInterruptRequest0x00(); // handle ignore
            static void HandleInterruptRequest0x01(); // handle keyboard
            static void HandleInterruptRequest0x0C(); // handle mouse
          


            void Deactivate();
    };
    } // namespace name
    
}

   

   
#endif
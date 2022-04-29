
#ifndef __INTERRUPTMANAGER_H
#define __INTERRUPTMANAGER_H

    #include <gdt.h>
    #include <types.h>
    #include <port.h>

    class InterruptManager
    {
        //friend class InterruptHandler;
        protected:

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


            static void InterruptIgnore();

            static void HandleInterruptRequest0x00();
            static void HandleInterruptRequest0x01();
          
            static uint32_t HandleInterrupt(uint8_t interrupt, uint32_t esp);

            Port8BitSlow PICMasterCommandPort;
            Port8BitSlow PICMasterDataPort;
            Port8BitSlow PICSlaveCommandPort;
            Port8BitSlow PICSlaveDataPort;

        public:
            InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable);
            ~InterruptManager();
            uint16_t HardwareInterruptOffset();
            void Activate();
            void Deactivate();
    };
#endif
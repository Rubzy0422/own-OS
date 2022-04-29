
#include "interrupts.h"


void printf(char* str);


InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];


void InterruptManager::SetInterruptDescriptorTableEntry(uint8_t interrupt,
    uint16_t CodeSegment, void (*handler)(), uint8_t DescriptorPrivilegeLevel, uint8_t DescriptorType)
{
    // address of pointer to code segment (relative to global descriptor table)
    // and address of the handler (relative to segment)
    interruptDescriptorTable[interrupt].handlerAddressLowBits = ((uint32_t) handler) & 0xFFFF;
    interruptDescriptorTable[interrupt].handlerAddressHighBits = (((uint32_t) handler) >> 16) & 0xFFFF;
    interruptDescriptorTable[interrupt].gdt_codeSegmentSelector = CodeSegment;

    const uint8_t IDT_DESC_PRESENT = 0x80;
    interruptDescriptorTable[interrupt].access = IDT_DESC_PRESENT | ((DescriptorPrivilegeLevel & 3) << 5) | DescriptorType;
    interruptDescriptorTable[interrupt].reserved = 0;
}


InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset, GlobalDescriptorTable* globalDescriptorTable)
    : PICMasterCommandPort(0x20),
      PICMasterDataPort(0x21),
      PICSlaveCommandPort(0xA0),
      PICSlaveDataPort(0xA1)
{
    this->hardwareInterruptOffset = hardwareInterruptOffset;
    uint32_t CodeSegment = globalDescriptorTable->CodeSegmentSelector();

    const uint8_t IDT_INTERRUPT_GATE = 0xE;
    for(uint8_t i = 255; i > 0; --i)
    {
        SetInterruptDescriptorTableEntry(i, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
        //handlers[i] = 0;
    }
    SetInterruptDescriptorTableEntry(0, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    //handlers[0] = 0;



    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    
    PICMasterCommandPort.Write(0x11);
    PICSlaveCommandPort.Write(0x11);

    // remap
    PICMasterDataPort.Write(hardwareInterruptOffset);
    PICSlaveDataPort.Write(hardwareInterruptOffset+8);

    PICMasterDataPort.Write(0x04);
    PICSlaveDataPort.Write(0x02);

    PICMasterDataPort.Write(0x01);
    PICSlaveDataPort.Write(0x01);

    PICMasterDataPort.Write(0x00);
    PICSlaveDataPort.Write(0x00);

    InterruptDescriptorTablePointer idt_pointer;
    idt_pointer.size  = 256*sizeof(GateDescriptor) - 1;
    idt_pointer.base  = (uint32_t)interruptDescriptorTable;
    asm volatile("lidt %0" : : "m" (idt_pointer));
}

InterruptManager::~InterruptManager()
{
    Deactivate();
}

uint16_t InterruptManager::HardwareInterruptOffset()
{
    return hardwareInterruptOffset;
}

void InterruptManager::Activate()
{
    //if(ActiveInterruptManager == 0)
    {
        //ActiveInterruptManager = this;
        asm("sti");
    }
}

void InterruptManager::Deactivate()
{
    /*if(ActiveInterruptManager == this)
    {
        ActiveInterruptManager = 0;
        */
        //asm("cli");
        /*
    }*/
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interrupt, uint32_t esp)
{
    char* foo = "INTERRUPT 0x00";
    char* hex = "0123456789ABCDEF";

    foo[12] = hex[(interrupt >> 4) & 0xF];
    foo[13] = hex[interrupt & 0xF];
    printf(foo);

    return esp;
}

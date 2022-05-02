
#include <hardwarecommunication/interrupts.h>

using namespace kernelos::common;
using namespace kernelos::hardwarecommunication;


void printf(char* value);
void printfHex(uint8_t key);

InterruptHandler::InterruptHandler( uint8_t interruptNumber, InterruptManager* interruptManager) {
    this->interruptNumber = interruptNumber;
    this->interruptManager = interruptManager;
    interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler() {
    if (interruptManager->handlers[interruptNumber] == this)
        interruptManager->handlers[interruptNumber] = 0;
}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp) {
    return esp;
}

InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];


InterruptManager* InterruptManager::ActiveInterruptManager = 0;

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
        handlers[i] = 0;
    }
    SetInterruptDescriptorTableEntry(0, CodeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
    //handlers[0] = 0;



    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x00, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x01, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
    SetInterruptDescriptorTableEntry(hardwareInterruptOffset + 0x0C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
    
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

    if(ActiveInterruptManager != 0)
        ActiveInterruptManager->Deactivate();
        ActiveInterruptManager = this;
        asm("sti");
    
}

void InterruptManager::Deactivate()
{

    if(ActiveInterruptManager == this)
    {
        ActiveInterruptManager = 0;
        asm("cli");
    }
}

uint32_t InterruptManager::HandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if (ActiveInterruptManager != 0) {
        ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
    }
    return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
    if (handlers[interruptNumber] != 0)
    {
        esp = handlers[interruptNumber]->HandleInterrupt(esp);
    }
    else if (interruptNumber != 0x20) {
        // print interrupt is empty

            char* foo = "INTERRUPT 0x";
            printf(foo);
            printfHex(interruptNumber);
        }
        
   

// Only send answers to hardware interrupts 0x20 - 0x30
    if (0x20 <= interruptNumber && interruptNumber < 0x30)
    {
        // Send the answer
        PICMasterCommandPort.Write(0x20);
        // only if this came from slave will we use this 
        if (0x28 <= interruptNumber)
            PICSlaveCommandPort.Write(0x20);
    }
    return esp;
}

#include <hardwarecommunication/pci.h>

using namespace kernelos::common;
using namespace kernelos::hardwarecommunication;

void printf(char* value);
void printfHex(uint8_t key);

PeripheralComponentInterfaceDeviceDescriptor::PeripheralComponentInterfaceDeviceDescriptor()
{}
PeripheralComponentInterfaceDeviceDescriptor::~PeripheralComponentInterfaceDeviceDescriptor()
{}

PeripheralComponentInterfaceDeviceDescriptor PeripheralComponentInterfaceController::getDeviceDescriptor(uint16_t bus,uint16_t device,uint16_t function)
{
    PeripheralComponentInterfaceDeviceDescriptor result;
    // could be a constructor but eh
    result.bus = bus;
    result.device = device;
    result.function = function;

// 
    result.vendor_id = Read(bus, device, function, 0x00);
    result.device_id = Read(bus, device, function, 0x02);
    
    result.class_id = Read(bus, device, function, 0x0B);
    result.subclass_id = Read(bus, device, function, 0x0A);
    result.interface_id = Read(bus, device, function, 0x09);
    
    result.revision = Read(bus, device, function, 0x08);
    result.interrupt = Read(bus, device, function, 0x3C);
    
    return result;
}
   



PeripheralComponentInterfaceController::PeripheralComponentInterfaceController(/* args */)
: dataPort(0xCFC),
  commandPort(0xCF8)
{
}
    
PeripheralComponentInterfaceController::~PeripheralComponentInterfaceController()
{
}

uint32_t PeripheralComponentInterfaceController::Read(uint16_t bus,uint16_t device,uint16_t function, uint32_t registeroffset) {
    // Construct identifier
    uint32_t id = 
    0x01 << 31
    | ((bus & 0xFF) << 16)
    | ((device & 0x1F) << 11)
    | ((function & 0x07) << 8)
    | ((registeroffset &0xFC));

    commandPort.Write(id);
    uint32_t result = dataPort.Read();
    return result >> (8* (registeroffset % 4));
}

void PeripheralComponentInterfaceController::Write(uint16_t bus,uint16_t device,uint16_t function, uint32_t registeroffset, uint32_t value) {
    uint32_t id = 
    0x01 << 31
    | ((bus & 0xFF) << 16)
    | ((device & 0x1F) << 11)
    | ((function & 0x07) << 8)
    | ((registeroffset &0xFC));
    
    commandPort.Write(id);
    dataPort.Write(value);
}

bool PeripheralComponentInterfaceController::DeviceHasFunctions(uint16_t bus,uint16_t device) {
    //if device has functions or not
    return Read(bus, device, 0 ,0x0E) & (1<<7);
}
            
void PeripheralComponentInterfaceController::SelectDrivers(kernelos::drivers::DriverManager* driverManager) {
    for (int bus=0; bus < 8 ; bus++) {// loop bus 
        for (int device = 0; device < 32; device++) { // loop devices
            int numFunctions = DeviceHasFunctions(bus, device) ? 0: 1;
            for (int function = 0; function < numFunctions; function++) // loop functions
            {
                PeripheralComponentInterfaceDeviceDescriptor dev = getDeviceDescriptor(bus,device,function);

                // Vendorid = 0 or all 1 if no function
                if (dev.vendor_id == 0x0000 || dev.vendor_id == 0xFFFF)
                    break;
                
                printf("PCI BUS ");
                printfHex(bus & 0xFF);

                printf(", DEVICE ");
                printfHex(device & 0xFF);

                printf(", FUNCTION ");
                printfHex(function & 0xFF);

                printf(", VENDOR ");
                printfHex((dev.vendor_id & 0xFF00) >> 8);
                printfHex(dev.vendor_id & 0xFF);

                printf(", DEVICE ");
                printfHex((dev.device_id & 0xFF00) >> 8);
                printfHex(dev.device_id & 0xFF);
                printf("\n");
            }
        }
    }        
}
            
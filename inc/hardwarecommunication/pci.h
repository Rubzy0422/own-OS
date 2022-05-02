#ifndef __HARDWARECOMMUNICATION__PCI_H
#define __HARDWARECOMMUNICATION__PCI_H

#include <hardwarecommunication/port.h>
#include <drivers/driver.h>
#include <common/types.h>
#include <hardwarecommunication/interrupts.h>

namespace kernelos {
    namespace hardwarecommunication {

        class PeripheralComponentInterfaceDeviceDescriptor
        {
            public:
                common::uint32_t portBase; // use for comm
                common::uint32_t interrupt;

                common::uint16_t bus; 
                common::uint16_t device; 
                common::uint16_t function; 

                common::uint16_t vendor_id; 
                common::uint16_t device_id;

                common::uint8_t class_id; 
                common::uint8_t subclass_id; 
                common::uint8_t interface_id; 

                common::uint16_t revision; 

                PeripheralComponentInterfaceDeviceDescriptor();
                ~PeripheralComponentInterfaceDeviceDescriptor();
        };

        class PeripheralComponentInterfaceController
            {
                Port32Bit dataPort;
                Port32Bit commandPort;
            public:
                PeripheralComponentInterfaceController(/* args */);
                ~PeripheralComponentInterfaceController();
                // Read Data From function 
                // Get offset of certain memory 

                common::uint32_t Read(common::uint16_t bus,common::uint16_t device,common::uint16_t function, common::uint32_t registeroffset);
                void Write(common::uint16_t bus,common::uint16_t device,common::uint16_t function, common::uint32_t registeroffset, common::uint32_t value);
                bool DeviceHasFunctions(common::uint16_t bus,common::uint16_t device);
            
                void SelectDrivers(kernelos::drivers::DriverManager* driverManager);
                PeripheralComponentInterfaceDeviceDescriptor getDeviceDescriptor(common::uint16_t bus,common::uint16_t device,common::uint16_t function);
            };

    }
}
        

#endif

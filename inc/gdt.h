#ifndef __GDT_H
#define __GDT_H 
    #define CODE_SEGMENT_SELECTOR_SIZE 64*1024*1024
    #define DATA_SEGMENT_SELECTOR_SIZE 64*1024*1024    
    #include <common/types.h>

namespace kernelos
{
    


    class GlobalDescriptorTable
    {
        public:
            class SegmentDescriptor
            {
                private:
                    kernelos::common::uint16_t limit_lo;
                    kernelos::common::uint16_t base_lo;
                    kernelos::common::uint8_t base_hi;
                    kernelos::common::uint8_t type;
                    kernelos::common::uint8_t flags_limit_hi;
                    kernelos::common::uint8_t base_vhi;
                public:
                    SegmentDescriptor(kernelos::common::uint32_t base, kernelos::common::uint32_t limit, kernelos::common::uint8_t type);
                    kernelos::common::uint32_t Base();
                    kernelos::common::uint32_t Limit();

            }   __attribute__((packed));

        SegmentDescriptor nullSegmentSelector;
        SegmentDescriptor unusedSegmentSelector;
        SegmentDescriptor codeSegmentSelector;
        SegmentDescriptor dataSegmentSelector;

        // Security conserne

    public:
        GlobalDescriptorTable();
        ~GlobalDescriptorTable();

        kernelos::common::uint16_t CodeSegmentSelector();
        kernelos::common::uint16_t DataSegmentSelector();
    };
} // namespace kernelos
#endif
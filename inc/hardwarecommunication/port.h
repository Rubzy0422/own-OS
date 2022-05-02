#ifndef __PORT_H
#define __PORT_H
    #include <common/types.h>

namespace kernelos
{
  namespace hardwarecommunication
  {
    class Port
    {
        protected:
            kernelos::common::uint16_t portnumber;
            Port(kernelos::common::uint16_t portnumber);
            ~Port();
    };






  class Port8Bit : public Port
  {

  public:
      Port8Bit(kernelos::common::uint16_t portnumber);
      ~Port8Bit();
      virtual void Write(kernelos::common::uint8_t data);
      virtual kernelos::common::uint8_t Read();
  };

class Port8BitSlow : public Port8Bit
  {

  public:
      Port8BitSlow(kernelos::common::uint16_t portnumber);
      ~Port8BitSlow();
      virtual void Write(kernelos::common::uint8_t data);
  };

    class Port16Bit : public Port
  {

  public:
      Port16Bit(kernelos::common::uint16_t portnumber);
      ~Port16Bit();
      virtual void Write(kernelos::common::uint16_t data);
      virtual kernelos::common::uint16_t Read();
  };
 
     class Port32Bit : public Port
  {

  public:
      Port32Bit(kernelos::common::uint32_t portnumber);
      ~Port32Bit();
      virtual void Write(kernelos::common::uint32_t data);
      virtual kernelos::common::uint32_t Read();
  };
 
  } // namespace hardwarecommunication
  
} // namespace kernelos

    
  
  
    
#endif
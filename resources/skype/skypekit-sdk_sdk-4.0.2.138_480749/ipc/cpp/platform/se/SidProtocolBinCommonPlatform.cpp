#include "SidPlatform.hpp"
#include "SidProtocolBinCommon.hpp"
#include "SidTransportInterface.hpp"
#include "SidProtocolFactory.hpp"
#include "SidField.hpp"

namespace Sid 
{
  namespace Protocol {
    Status BinCommon::rd_value(CommandInitiator* thread, String& value)  
    {
      uint size;
      Status status = rd_value(thread, size);
      if (status == OK && size > 0) {
        value = String(size+1);
        if ((status = rd_bytes(thread, size, (char*)value.data())) != OK) return status;
        ((char*)value.data())[size]=0;
      } else {
        value = String();
      }
      return status;   
    }

    Status BinCommon::wr_value(CommandInitiator* thread, const String& val)
    {
      uint size = val.size();
      if (wr_value(thread, size) != OK) return ERR_ENCODE;
      if (size > 0)
        if (m_transport->bl_write_bytes(thread, size,val.data()) != TransportInterface::OK) return ERR_ENCODE;
      return OK;
    }

    Status BinCommon::rd_value(CommandInitiator* thread, Filename& value)  
    {
      uint size;
      Status status = rd_value(thread, size);
      if (status == OK && size > 0) {
        value = Filename(size+1);
        if ((status = rd_bytes(thread, size, (char*)value.data())) != OK) return status;
        ((char*)value.data())[size]=0;
      } else {
        value = Filename();
      }
      return status;   
    }

    Status BinCommon::wr_value(CommandInitiator* thread, const Filename& val)
    {
      return wr_value(thread, (const String&) val) ;
    }

  } // Protocol


} //namespace


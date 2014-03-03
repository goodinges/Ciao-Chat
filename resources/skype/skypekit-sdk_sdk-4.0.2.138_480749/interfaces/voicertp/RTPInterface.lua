require 'xsid'

-- Decorators
RETURN = "RETURN"



multirtpif = SID "SkypeMultiRTPInterface" {  -- skylib2ve

    STRUCT "Codec"
    {
      FIELD(10) "Name"                         { type = "string" },
      FIELD(11) "PayloadType"                 { type = "int" },
      FIELD(12) "Freq"                         { type = "int" },
      FIELD(13) "Channels"                 { type = "int" },
      FIELD(14) "SupportedPacketSizes"         { type = LIST "uint" },
      FIELD(15) "DefaultPacketSize"         { type = "int" },
      FIELD(16) "MinBitrate"                 { type = "int" },
      FIELD(17) "MaxBitrate"                 { type = "int" },
      FIELD(18) "MaxDesiredPacketLoss"       { type = "int" },
    };


    ACTION(1) "GetVersion" 
      { 
      }
    - { 
        FIELD(1) "version" { type = "string"} ,
        FIELD(2,RETURN) "rv" {type = "int"}
      };
    ACTION(2) "GetLastError" 
      { }
    - { 
        FIELD(1, RETURN) "error" { type = "int" } ,
      };
    ACTION(3) "Init" 
      { 
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };
    ACTION(4) "Uninit" 
      { 
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };


    ACTION(10) "CreateChannel" 
      { 
        FIELD(1) "call_member_id"  { type = "uint" },
        FIELD(2) "conversation_id" { type = "uint" },
      }
      - { 
        FIELD(1) "channel" { type = "int" } ,
        FIELD(2,RETURN) "rv" { type = "int" } 
      };

    ACTION(11) "DeleteChannel" 
      { 
        FIELD(1, RETURN) "channel_id"  { type = "int" } }
      -{
        FIELD(1, RETURN) "rv" {type = "int"}
      };
    ACTION(12) "ChangeConversationId" 
      { 
        FIELD(1) "channel_id"  { type = "int" }, 
        FIELD(2) "conversation_id"  { type = "uint" },
        FIELD(3) "call_member_id"  { type = "uint" }
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };


    ACTION(20) "GetCodec"
      { FIELD(1) "id" { type = "uint"},}
    - { 
        FIELD(1) "codec" { type = "Codec"},
--[[        
      FIELD(10) "Name"                         { type = "string" },
      FIELD(11) "PayloadType"                 { type = "int" },
      FIELD(12) "Freq"                         { type = "int" },
      FIELD(13) "Channels"                 { type = "int" },
      FIELD(14) "SupportedPacketSizes"         { type = LIST "uint" },
      FIELD(15) "DefaultPacketSize"         { type = "int" },
      FIELD(16) "MinBitrate"                 { type = "int" },
      FIELD(17) "MaxBitrate"                 { type = "int" },
]]--
        FIELD(20, RETURN) "rv" { type = "uint"},
      };
    ACTION(21) "GetCodecCount"
      {}
    - { 
        FIELD(1) "count" { type = "uint"},
        FIELD(2, RETURN) "rv" { type = "int"},
      };
    ACTION(22) "SetSendCodec" 
      { 
        FIELD(1) "channel_id"  { type = "int" },
        FIELD(2) "Name"  { type = "string" },
        FIELD(3) "PacketSize"  { type = "int" },
        FIELD(4) "BitRate"  { type = "int" },
        FIELD(5) "PayloadType"  { type = "int" },
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };
    ACTION(23) "InformSendPacketLoss"
      {
	FIELD(1) "channel_id" { type = "int" },
	FIELD(2) "packetLossBeforeFEC" { type = "uint" },
	FIELD(3) "packetLossAfterFEC" { type = "uint" },
	FIELD(4) "jitterBeforeFEC" { type = "uint" },
	FIELD(5) "jitterAfterFEC" { type = "uint" },
	
--      }
--      -{
      };
      
      
    ACTION(30) "StartPlayback" 
      { 
        FIELD(1) "channel_id"  { type = "int" },
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };
    ACTION(31) "StopPlayback" 
      { 
        FIELD(1) "channel_id"  { type = "int" },
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };
    ACTION(32) "StartRecording" 
      { 
        FIELD(1) "channel_id"  { type = "int" },
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };
    ACTION(33) "StopRecording" 
      { 
        FIELD(1) "channel_id"  { type = "int" },
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };

    ACTION(40) "SubmitRTPPacket" 
      { 
        FIELD(1) "channel_id"  { type = "int" },
        FIELD(2) "data"  { type = "binary" },
--      }
--      -{
--        FIELD(1,RETURN) "rv" {type = "int"}
      };
    ACTION(41) "SendDTMF" 
      { 
        FIELD(1) "channel_id"  { type = "int" },
        FIELD(2) "eventnr"  { type = "int" },
        FIELD(3) "rtp_type"  { type = "int" },
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };

    ACTION(50) "SetOutputAmplification" 
      {
        FIELD(1) "channel_id"  { type = "int" },
        FIELD(2) "scale"  { type = "int" },
      }
      -{
        FIELD(1,RETURN) "rv" {type = "int"}
      };
    ACTION(51) "CustomCommand" 
      { 
        FIELD(1) "command"  { type = "string" },
      }
    - { 
        FIELD(1) "result_str" { type = "string" },
        FIELD(2, RETURN) "rv" { type = "int" },
      };

}

multirtpif_cb = SID "SkypeMultiRTPCallbackInterface" {  -- skylib2ve
    ACTION(1) "SendRTPPacket" 
      { 
        FIELD(1) "channel_id"  { type = "int" },
        FIELD(2) "data"  { type = "binary" },
      };
}

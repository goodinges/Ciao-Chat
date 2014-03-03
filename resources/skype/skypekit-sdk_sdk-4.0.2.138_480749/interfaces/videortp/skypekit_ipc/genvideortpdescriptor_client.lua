package.path = package.path .. ';../../../../sid/sid/?.lua;../../../../sid/interfaces/avipc_common/?.lua;;../../../../sid/interfaces/videortp/?.lua'

require 'VideoRTPInterface'
require 'genavipcdescriptor_client'
require 'genavipcdescriptor_server'

ifheader = 'SidVideoRTPInterface.hpp'
if_transport_fail_code = 0
--fordebug = 1
 
videortpif:generate_client_descriptor(fordebug, "videortpif","GI") --, io.stdout)
videortpif_cb:generate_server_descriptor(fordebug, "videortpif_cb","GI") --, io.stdout)

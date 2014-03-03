package.path = package.path .. ';../?.lua;../../../sid/?.lua;../../avipc_common/?.lua;../../../utils/lua/?.lua'

require 'VideoRTPInterface'
require 'genavipcdescriptor_server'
require 'genavipcdescriptor_client'

ifheader = 'SidVideoRTPInterface.hpp'
if_transport_fail_code = 0

--fordebug = 1

videortpif:generate_server_descriptor(fordebug, "videortpif","SE") --, io.stdout)
videortpif_cb:generate_client_descriptor(fordebug, "videortpif_cb", "SE") --, io.stdout)


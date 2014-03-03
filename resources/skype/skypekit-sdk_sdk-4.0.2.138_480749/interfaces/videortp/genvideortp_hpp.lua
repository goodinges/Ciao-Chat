package.path = package.path .. ';../avipc_common/?.lua'
require 'VideoRTPInterface'
require 'generate_hpp'

generator = Generator {
		--generate_comments = true,
		indentation_step = 1,
		indent_with_tabs = true,
}

generator:write([[
/*
* Copyright (C) 2007-2009 Skype Technologies S.A. Confidential and proprietary
*
* All intellectual property rights, including but not limited to copyrights,
* trademarks and patents, as well as know how and trade secrets contained
* in, relating to, or arising from the internet telephony software of Skype
* Limited (including its affiliates, "Skype"), including without limitation
* this source code, Skype API and related material of such software
* proprietary to Skype and/or its licensors ("IP Rights") are and shall
* remain the exclusive property of Skype and/or its licensors. The recipient
* hereby acknowledges and agrees that any unauthorized use of the IP Rights
* is a violation of intellectual property laws.
*
* Skype reserves all rights and may take legal action against infringers of
* IP Rights.
*
* The recipient agrees not to remove, obscure, make illegible or alter any
* notices or indications of the IP Rights and/or Skype's rights and ownership
* thereof.
*/
#include "SidPlatform.hpp"

#ifndef __SkypeVideoRTPInterface_hpp__
#define __SkypeVideoRTPInterface_hpp__

#ifndef MAKEFOURCC
#if defined(__IPP3__) || defined(__ARMEB__) || defined(__MIPSEB__)
	#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
		        ((unsigned int)(unsigned char)(ch3) | ((unsigned int)(unsigned char)(ch2) << 8) |   \
		        ((unsigned int)(unsigned char)(ch1) << 16) | ((unsigned int)(unsigned char)(ch0) << 24 ))
#else
	#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
		        ((unsigned int)(unsigned char)(ch0) | ((unsigned int)(unsigned char)(ch1) << 8) |   \
		        ((unsigned int)(unsigned char)(ch2) << 16) | ((unsigned int)(unsigned char)(ch3) << 24 ))
#endif
#endif                  
                  
]])	
	
videortpif:generate_hpp("videortpif", generator)

videortpif_cb:generate_hpp("videortpif_cb", generator)

generator:write([[

/*!
 * Skypekit calls this method to create an instance of PVE. Skypekit 
 * passes a callback to this method to enable PVE to call methods on skypekit side  
 */
extern "C" SkypeVideoRTPInterface * SkypeVideoRTPInterfaceGet(SkypeVideoRTPInterfaceCb *callback);

extern "C" void SkypeVideoRTPInterfaceRelease(SkypeVideoRTPInterface* rtpvideo);

#endif /* !__SkypeRTPInterface_hpp__ */
]])																							
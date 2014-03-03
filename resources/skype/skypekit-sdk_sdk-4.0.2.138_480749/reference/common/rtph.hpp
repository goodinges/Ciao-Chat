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

#ifndef __rtph_h__
#define __rtph_h__

#include <arpa/inet.h>
#include <string.h>

/**
*   The RTP header has the following format:
*
*  0                   1                   2                   3
*  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |V=2|P|X|  CC   |M|     PT      |       sequence number         |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |                           timestamp                           |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
* |           synchronization source (SSRC) identifier            |
* +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
* |            contributing source (CSRC) identifiers             |
* |                             ....                              |
* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/
/// RTP header, see RFC 3550
struct rtph {
	unsigned char Data[12];

	inline unsigned int GetVersion(void) const {return Data[0]>>6;}
	inline unsigned int GetPadding() const		{return Data[0]>>5&0x01;}
	inline unsigned int GetExtension() const	{return Data[0]>>4&0x01;}
	inline unsigned int GetMarker() const		{return Data[1]>>7;}
	inline unsigned int GetPayloadType(void) const {return Data[1]&0x7f;}
	inline unsigned int GetSequence(void) const {return (Data[2]<<8) | Data[3];}
	inline unsigned int GetTimestamp(void) const {return ((unsigned int)Data[4]<<24)|((unsigned int)Data[5]<<16)|((unsigned int)Data[6]<<8)|Data[7];}
	inline unsigned int GetSSRC(void) const {return ((unsigned int)Data[8]<<24)|((unsigned int)Data[9]<<16)|((unsigned int)Data[10]<<8)|Data[11];}
	inline unsigned int GetCSRCCount(void) const {return Data[0]&0xf;}
	inline bool IsCompressable(void) const {return Data[0]==0x80;}

	inline void Reset(void){memset(Data,0,sizeof(Data));Data[0]=0x80;}
	inline void SetMarker(unsigned int marker){Data[1] = (Data[1] & 0x7F) | (marker<<7);}
	inline void SetPayloadType(unsigned int type){Data[1]=(Data[1]&0x80)|(type&0x7f);}
	inline void SetSequence(unsigned short seq){Data[3]=(unsigned char)seq;Data[2]=(unsigned char)(seq>>8);}
	inline void SetTimestamp(unsigned int t){Data[4]=(unsigned char)(t>>24);Data[5]=(unsigned char)(t>>16);Data[6]=(unsigned char)(t>>8);Data[7]=(unsigned char)t;}
	inline void SetSSRC(unsigned int ssrc){Data[8]=(unsigned char)(ssrc>>24);Data[9]=(unsigned char)(ssrc>>16);Data[10]=(unsigned char)(ssrc>>8);Data[11]=(unsigned char)ssrc;}
	inline unsigned char *GetDataPtr(void){return Data+sizeof(Data);}
} __attribute__ ((packed));



#endif //__rtph_h__


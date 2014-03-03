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

#ifndef RFC3984_HPP_
#define RFC3984_HPP_

#include <stdint.h>

struct NalHeader {
	uint8_t data;

	uint8_t GetF() {return data >> 7;}
	uint8_t GetNri() {return data >> 5 & 0x3;}
	uint8_t GetType() {return data & 0x1f;}

	void SetNri(uint8_t nri) {data = (data & 0x9F)|(nri << 5 & 0x60);}
	void SetType(uint8_t type) {data = (data & 0xE0)|(type & 0x1f);}
	void Reset() {data = 0;}
	uint8_t *GetDataPtr(void){return &data + sizeof(data);}
} __attribute__ ((packed));

struct FUAHeader {
	uint8_t data[2];

	uint8_t GetF() {return data[0] >> 7;}
	uint8_t GetNri() {return data[0] >> 5 & 0x3;}
	uint8_t GetFuaType() {return data[0] & 0x1f;}
	uint8_t GetS() {return data[1] >> 7;}
	uint8_t GetE() {return data[1] >> 6 & 0x1;}
	uint8_t GetR() {return data[1] >> 5 & 0x1;}
	uint8_t GetNalType() {return data[1] & 0x1f;}

	void SetNri(uint8_t nri) {data[0] = (data[0] & 0x9F)|(nri << 5 & 0x60);}
	void SetS(uint8_t s) {data[1] = (data[1] & 0x7f)|(s << 7);}
	void SetE(uint8_t e) {data[1] = (data[1] & 0xbf)|(e << 6 & 0x40);}
	void SetNalType(uint8_t type) {data[1] = (data[1] & 0xE0)|(type & 0x1f);}
	void Reset() {data[0] = 28; data[1] = 0;}	
	uint8_t *GetDataPtr(void){return data + sizeof(data);}
} __attribute__ ((packed));

enum nalu_type
{
    NALU_UNDEFINED_00 = 0,

    NALU_SLICE		 = 1,
    NALU_SLICE_DPA   = 2,
    NALU_SLICE_DPB   = 3,
    NALU_SLICE_DPC   = 4,
    NALU_SLICE_IDR   = 5,    /* ref_idc != 0 */
    NALU_SEI         = 6,    /* ref_idc == 0 */
    NALU_SPS         = 7,
    NALU_PPS         = 8,
    NALU_AUD         = 9,
    NALU_EOSEQ       = 10,
    NALU_EOSTRM	     = 11,
    NALU_FILLER      = 12,
    /* ref_idc == 0 for 6,9,10,11,12 */

	NALU_STAP_A		= 24,
	NALU_STAP_B		= 25,
	NALU_MTAP16		= 26,
	NALU_MTAP24		= 27,
	NALU_FU_A		= 28,
	NALU_FU_B		= 29
};

#define MAX_STAPA_PACKET_PAYLOAD 65535

#endif /* RFC3948_HPP_ */

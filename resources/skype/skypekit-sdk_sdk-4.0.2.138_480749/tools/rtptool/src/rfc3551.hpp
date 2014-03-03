/*
 * Copyright (C) 2010 Skype Technologies S.A. Confidential and proprietary
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

#ifndef rfc3551_hpp_
#define rfc3551_hpp_

enum payload_type {
	PT_PCMU       = 0,
	PT_PCMA       = 8,
	PT_G729       = 18,
	PT_DYNAMIC    = 96,
	PT_SILK_V3    = 100,
	PT_SILK_V3_MB = 102,
	PT_SILK_V3_NB = 103,
	PT_SILK_V3_WB = 104
};

#endif /* rfc3551_hpp_ */

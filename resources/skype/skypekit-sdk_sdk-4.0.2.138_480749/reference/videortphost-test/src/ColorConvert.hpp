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

#ifndef COLOURCONVERT_HPP_
#define COLOURCONVERT_HPP_

#include <stdint.h>

/*! \class ColorConvert
 *  Implements various color conversions.
 */
class ColorConvert {
public:
	static void ConvertYUY2toI420(int inWidth, int inHeight, 
						 uint8_t *pSrc, uint8_t *pDest);
	static inline int YUVtoRGB(int y, int u, int v);
	static void ConvertI420toRGB(int inWidth, int inHeight, 
			uint8_t *pSrc, uint8_t *pDest);
};

#endif /* COLOURCONVERT_HPP_ */

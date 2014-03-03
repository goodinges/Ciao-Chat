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

#include "ColorConvert.hpp"

void ColorConvert::ConvertYUY2toI420(int inWidth, int inHeight, uint8_t *pSrc, uint8_t *pDest)
{
	int i, j;

	uint8_t *y = pDest;
	uint8_t *u = y + (inWidth * inHeight);
	uint8_t *v = u + (inWidth * inHeight) / 4;

	for (i = 0; i < inHeight; i++) {
		for (j = 0; j < inWidth / 2; j++) {
			// two pels in one go

			// Y1 U Y2 V layout
			if (i % 2) {
				*y++ = *pSrc++;
				*u++ = *pSrc++;
				*y++ = *pSrc++;
				*v++ = *pSrc++;
			} else {
				*y++ = *pSrc++;
				pSrc++; // jump over u
				*y++ = *pSrc++;
				pSrc++; // jump over v				
			}
		}
	}
}

#define CLIP(x) if (x > 255) {x = 255;} if (x < 0) {x = 0;}

inline int ColorConvert::YUVtoRGB(int y, int u, int v)
{
	int r, g, b;

	const int shl16_1point404 = (int) (1.404 * (1 << 16));
	const int shl16_0point344 = (int) (0.344 * (1 << 16));
	const int shl16_0point714 = (int) (0.714 * (1 << 16));
	const int shl16_1point772 = (int) (1.772 * (1 << 16));

	u -= 128;
	v -= 128;

	r = (y + ((shl16_1point404 * v) >> 16));
	g = (y - (((shl16_0point344 * u) + (shl16_0point714 * v)) >> 16));
	b = (y + ((shl16_1point772 * u) >> 16));

	CLIP(r);
	CLIP(g);
	CLIP(b);

	return ((r << 16) + (g << 8) + b);
}

void ColorConvert::ConvertI420toRGB(int inWidth, int inHeight, uint8_t *pSrc, uint8_t *pDest)
{
	int pos, rgbpix, i, j;

	uint8_t *y = pSrc;
	uint8_t *u = y + (inWidth * inHeight);
	uint8_t *v = u + (inWidth * inHeight) / 4;

	for (i = 0; i < inHeight; i++) {
		for (j = 0; j < inWidth; j++) {
			int uvalue = u[((i / 2 * inWidth / 2)) + (j / 2)];
			int vvalue = v[((i / 2 * inWidth / 2)) + (j / 2)];

			rgbpix = YUVtoRGB(y[i * inWidth + j], uvalue, vvalue);

			pos = (i * inWidth) + (j);
			((uint32_t *) pDest)[pos] = rgbpix;
		}
	}
}


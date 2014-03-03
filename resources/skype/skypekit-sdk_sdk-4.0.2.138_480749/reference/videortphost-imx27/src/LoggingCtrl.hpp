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

#ifndef LOGGINGCTRL_HPP_
#define LOGGINGCTRL_HPP_

#define ENCODER_DBG_LEVEL 0x20
#define DECODER_DBG_LEVEL 0x40
#define CAPTURE_DBG_LEVEL 0x80
#define RENDER_DBG_LEVEL 0x100
#define RTPIF_DBG_LEVEL 0x200
#define TIMING_DBG_LEVEL 0x400
#define PACKET_DBG_LEVEL 0x800

#undef LOG_MASK

#define LOG_MASK (0xf | RTPIF_DBG_LEVEL)

#undef USE_TIMESTAMPS
#define USE_TIMESTAMPS 1

#undef GLOBAL_PREFIX
#define GLOBAL_PREFIX "videohost"

#include "debugging.hpp"

#define ENC_DBG(fmt, args...) LOG(ENCODER_DBG_LEVEL, "Encoder:" fmt, ##args)
#define DEC_DBG(fmt, args...) LOG(DECODER_DBG_LEVEL, "Decoder:" fmt, ##args)
#define CAP_DBG(fmt, args...) LOG(CAPTURE_DBG_LEVEL, "Capture:" fmt, ##args)
#define REND_DBG(fmt, args...) LOG(RENDER_DBG_LEVEL, "Render:" fmt, ##args)
#define RTP_DBG(fmt, args...) LOG(RTPIF_DBG_LEVEL, "TestIF:" fmt, ##args)
#define TIMING_DBG(fmt, args...) LOG(TIMING_DBG_LEVEL, "Timing:" fmt, ##args)
#define PACKET_DBG(fmt, args...) LOG(PACKET_DBG_LEVEL, "Packets:" fmt, ##args)

#define RTP_FUNCLOG FuncLog __l(__func__, "TI_F:", USE_TIMESTAMPS, LOG_MASK, RTPIF_DBG_LEVEL);

#endif /* LOGGINGCTRL_HPP_ */

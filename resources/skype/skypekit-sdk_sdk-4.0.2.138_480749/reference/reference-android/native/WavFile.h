/**
 * Copyright (C) 2011, Skype Limited
 *
 * All intellectual property rights, including but not limited to copyrights,
 * trademarks and patents, as well as know how and trade secrets contained in,
 * relating to, or arising from the internet telephony software of
 * Skype Limited (including its affiliates, "Skype"), including without
 * limitation this source code, Skype API and related material of such
 * software proprietary to Skype and/or its licensors ("IP Rights") are and
 * shall remain the exclusive property of Skype and/or its licensors.
 * The recipient hereby acknowledges and agrees that any unauthorized use of
 * the IP Rights is a violation of intellectual property laws.
 *
 * Skype reserves all rights and may take legal action against infringers of
 * IP Rights.
 *
 * The recipient agrees not to remove, obscure, make illegible or alter any
 * notices or indications of the IP Rights and/or Skype's rights and
 * ownership thereof.
 */

#ifndef WAVFILE_H_
#define WAVFILE_H_

struct WaveHeader {
        char            riff[4];                // 'RIFF'
        uint32_t                     size;                   // Size of the file
        char            wave[4];                // 'WAVE'
        // fmt chunk
        char            fmt[4];                 // 'fmt '
        uint32_t        fmtSize;                // Chunk size
        uint16_t        fmtFormatTag;   // Format of this file
        uint16_t        fmtChannels;    // Num channels
        uint32_t        fmtSamPerSec;   // Samples per second
        uint32_t        fmtBytesPerSec; // Bytes per second
        uint16_t        fmtBlockAlign;  // Block alignment
        uint16_t        fmtBitsPerSam;  // Bits per sample
} __attribute__ ((packed));
struct chunkHeader {
        char            name[4];
        uint32_t                     size;
} __attribute__ ((packed));


#endif /* WAVFILE_H_ */

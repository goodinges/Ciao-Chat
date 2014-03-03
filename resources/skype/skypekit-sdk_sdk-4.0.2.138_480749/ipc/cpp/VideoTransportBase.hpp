#ifndef _VIDEOTRANSPORTBASE_H_
#define _VIDEOTRANSPORTBASE_H_

#pragma pack(push)
#pragma pack(1)

#ifdef DEBUG
#include <stdio.h>
#endif

class VideoTransportBase {
	public:
		VideoTransportBase() : meminfo( 0 ) {}
		enum BufferType {
			SysVBuffers,
			PosixBuffers,
			WinBuffers
		};
		typedef struct {
			int bufid;
			int size;
			int width;
			int height;
			int bitsperpixel;
			unsigned int fourcc;
			int orientation;
			long long clientpointer;
		} bufferstruct;

	protected:
		typedef struct {
			int bufferstates;
			bufferstruct buffers[2];
			BufferType buftype;
			int configuration;
			int fourcccount;
			unsigned int fourccs[10];
		} CommunicationSharedInfo;

		CommunicationSharedInfo *meminfo;

#ifdef DEBUG
		void dump() {
			printf("bufferstates: %i, %x\n", meminfo->bufferstates, meminfo->bufferstates );
			printf("buftype: %i, %x\n", meminfo->buftype, meminfo->buftype );
			printf("configuration: %i, %x\n", meminfo->configuration, meminfo->configuration );
			printf("fourcccount: %i\n", meminfo->fourcccount );
		}
#endif
};

#pragma pack(pop)

#endif

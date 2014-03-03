
#include <VideoTransportBase.hpp>

#ifdef _WIN32
#include <WinShm.hpp>
#else
#include <SysVShm.hpp>
#endif
#include <string.h>

template <class CommSharedMem>
class VideoTransportClient : public VideoTransportBase
{
	public:
		VideoTransportClient() : VideoTransportBase(),
			haveframe( false ) {
				CreateMemory();
		}
		~VideoTransportClient() {
			bufferstruct *b = 0;
			for( int i=0;i<2;i++ ) {
				b = &meminfo->buffers[i];
				if ( !b )
					continue;
				if ( b->clientpointer ) {
					CommSharedMem *s = (CommSharedMem *)b->clientpointer;
					s->unmap();
					delete s;
					b->clientpointer = 0;
				}
			}
			ipcmem.destroy();
		}

		bool CreateMemory() {
			bool ret =ipcmem.create(sizeof( CommunicationSharedInfo ));
			if ( !ret )
				return ret;
			meminfo = (CommunicationSharedInfo *)ipcmem.data();
			meminfo->bufferstates = 1;
			meminfo->buffers[0].bufid = -1;
			meminfo->buffers[1].bufid = -1;
			meminfo->buffers[0].clientpointer = 0;
			meminfo->buffers[1].clientpointer = 0;
			meminfo->fourcccount = 0;
			const char *memname = smem.getName();
			if (strcmp( memname, "SysV" ) == 0)
				meminfo->buftype = SysVBuffers;
			else if (strcmp( memname, "Win" ) == 0)
				meminfo->buftype = WinBuffers;
			else if (strcmp( memname, "Posix" ) == 0)
				meminfo->buftype = PosixBuffers;
			return true;
		}

		void setPreferences( int fourcccount, unsigned int *fourccs ) {
			if ( !meminfo )
				return;
			meminfo->fourcccount = fourcccount;
			for( int i=0;i<10 && i < fourcccount;i++ ) {
				meminfo->fourccs[i] = fourccs[i];
			}
		}

		int getBufStates() {
			return meminfo->bufferstates;
		}

		int key() {
			return ipcmem.key();
		};

		bool isNewFrameAvailable() {
			int bstate = (meminfo->bufferstates&0x3);
			if ( bstate != 0x00 && bstate != 0x3 )
				return false;
			return true;
		}

		bufferstruct *getNewFrame() {
			int bstate = (meminfo->bufferstates&0x3);
			if ( bstate != 0x00 && bstate != 0x3 )
				return 0;
			return getFrame();
		}
		bufferstruct *getFrame() {
			int bstate = (meminfo->bufferstates&0x3);
			if ( !haveframe && bstate == 0x1 )
				return 0;
			if ( !haveframe )
				haveframe = true;
			if ( bstate == 0x0 )
				meminfo->bufferstates |= 0x2;
			if ( bstate == 0x3 )
				meminfo->bufferstates &= ~0x2;
			bufferstruct *b = 0;
			bstate = (meminfo->bufferstates&0x3);
			if ( bstate == 0x1 )
				b = &meminfo->buffers[0];
			else if ( bstate == 0x2 )
				b = &meminfo->buffers[1];
			if ( !b )
				return 0;
			if ( b->clientpointer ) {
				CommSharedMem *s = (CommSharedMem *)b->clientpointer;
				if ( s->key() != b->bufid ) {
					s->unmap();
					delete s;
					b->clientpointer = 0;
				}
				else //even for same bufid, there is a chance the runtime reallocate memory for different buffer size
				{
					int size = static_cast<int>( s->getSize() );
					int width = b->width;
					int height = b->height;
					int bytesperpixel = b->bitsperpixel>>3;
					// check the sharememory buffer size is no less than the frame size in bytes
					if (size >= width * height * bytesperpixel )
					{
						b->clientpointer = (long long) s;
					}
					else
					{
						s->unmap();
						delete s;
						b->clientpointer = 0;
					}
				}
			}
			if ( !b->clientpointer ) {
				CommSharedMem *s = new CommSharedMem;
				s->map( b->bufid );
				int size = static_cast<int>( s->getSize() );
				int width = b->width;
				int height = b->height;
				int bytesperpixel = b->bitsperpixel>>3;
				// check the sharememory buffer size is no less than the frame size in bytes
				if (size >= width * height * bytesperpixel )
				{
					b->clientpointer = (long long) s;
				}
				else
				{
					s->unmap();
				    delete s;
					b->clientpointer = 0;
				}
			}
			return b;
		}

		void *bufferData( bufferstruct *b ) {
			CommSharedMem *s = (CommSharedMem *)b->clientpointer;
			return s->data();
		}

	private:
		CommSharedMem smem;

#ifdef _WIN32
		WinShm ipcmem;
#else
		SysVShm ipcmem;
#endif

		bool haveframe;
};

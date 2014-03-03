#ifndef _SYSVSHM_H_
#define _SYSVSHM_H_

#include <sys/shm.h>

class SysVShm {
	public:
		SysVShm();
		~SysVShm();
		bool create( size_t size );
		bool destroy();
		bool unmap();
		bool map( int atshmid = -1 );
		int key();
		void *data();
		static const char *getName();
        size_t getSize();

	private:
		int shmid;
		void *shmaddr;
		size_t datasize;
};
#endif

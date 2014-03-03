#ifndef _POSIXSHM_H_
#define _POSIXSHM_H_

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

class PosixShm {
	public:
		PosixShm();
		~PosixShm();
		bool create( size_t size );
		bool destroy();
		bool unmap();
		bool map( int atshmid = -1 );
		int key();
		void *data();
		static const char *getName();
		size_t getSize();

	private:
		char fname[80];
		int fd;
		int keynum;
		void *datap;
		size_t datasize;
};
#endif

#ifndef _WINSHM_H_
#define _WINSHM_H_

#include <windows.h>

class WinShm {
	public:
		WinShm();
		bool create( size_t size );
		bool destroy();
		bool unmap();
		bool map( int atshmid = -1 );
		int key();
		void *data();
		static const char *getName();
		size_t getSize();

	private:
		HANDLE hFile;
		void *datap;
		int id;
		size_t m_size;
};

#endif

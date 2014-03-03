#include "PosixShm.hpp"

#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>

PosixShm::PosixShm() :
	fd( -1 ),
	datasize( 0 )
{
}

PosixShm::~PosixShm()
{
}

bool PosixShm::create( size_t size )
{
	if ( fd != -1 )
		return false;
	keynum = 0;
	while( fd == -1  && keynum < 1000) {
		sprintf( fname, "skype_video_frame_buf_%i", ++keynum );
		fd = shm_open( fname, O_RDWR|O_CREAT|O_EXCL, 0600 );
	}
        if ( fd == -1 )
                return false;
        if ( ftruncate( fd, size ) < 0 ) {
                close( fd );
                fd = -1;
                shm_unlink( fname );
                return false;
        }

	datap = mmap( 0, size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0 );
	datasize = size;
	return true;
}

bool PosixShm::destroy()
{
	if ( fd == -1 )
		return false;
	munmap( datap, datasize );
	close( fd );
	shm_unlink( fname );
	fd = -1;
	datasize = 0;
	return true;
}

bool PosixShm::unmap()
{
	if ( fd == -1 )
		return false;
	munmap( datap, datasize );
	close( fd );
	fd = -1;
	datasize = 0;
	return true;
}

bool PosixShm::map( int mid )
{
	if ( mid == -1 || fd != -1 )
		return false;
	keynum = mid;
	sprintf( fname, "skype_video_frame_buf_%i", keynum );
	fd = shm_open( fname, O_RDWR|O_EXCL, 0600 );
	if ( fd == -1 )
		return false;
	struct stat sbuf;
	fstat( fd, &sbuf );
	datap = mmap( 0, sbuf.st_size, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0 );
	datasize = sbuf.st_size;
	return true;
}

int PosixShm::key()
{
	return keynum;
}

void *PosixShm::data()
{
	return datap;
}

const char *PosixShm::getName()
{
	return "Posix";
}

size_t PosixShm::getSize()
{
        return datasize;
}

#include "SysVShm.hpp"

//define a large adhoc size (1280*720*3*10 + 1) here for mapping case
#define __UNOBTAINED_SIZE__  27648001

SysVShm::SysVShm() :
	shmid( -1 ),
	shmaddr( 0 ),
	datasize( 0 )
{
}

SysVShm::~SysVShm()
{
}

bool SysVShm::destroy()
{
	unmap();
	if ( shmid != - 1 )
		shmctl( shmid, IPC_RMID, 0 );
	datasize = 0;
	return true;
}

bool SysVShm::unmap()
{
	if ( shmaddr ) {
		shmdt( shmaddr );
		shmaddr = 0;
	}
	datasize = 0;
	return true;
}

bool SysVShm::create( size_t size )
{
	shmid = shmget( IPC_PRIVATE, size, IPC_CREAT|0777);
	datasize = size;
	if ( shmid == -1 )
	{
		datasize = 0;
		return false;
	}
	return map( shmid );
}

bool SysVShm::map( int atshmid )
{
	if ( atshmid == -1 )
		atshmid = shmid;
	if ( atshmid == -1 )
	{
		datasize = 0;
		return false;
	}
	if ( shmid == -1 )
		shmid = atshmid;
	shmaddr = shmat( atshmid, 0, 0 );
	if ( shmaddr == (void *)-1 )
	{
		shmaddr = 0;
		datasize = 0;
	}
	else
	{
		datasize = __UNOBTAINED_SIZE__;
	}
	return ( shmaddr != 0 );
}

int SysVShm::key( void )
{
	return shmid;
}

void *SysVShm::data()
{
	return shmaddr;
}

const char *SysVShm::getName()
{
	return "SysV";
}

size_t SysVShm::getSize()
{
	return datasize;
}
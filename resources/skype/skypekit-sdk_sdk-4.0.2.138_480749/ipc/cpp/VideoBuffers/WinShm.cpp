#include <stdio.h>
#include <tchar.h>

#include "WinShm.hpp"

//as the windows don't provide API to get the mapping file size
//define a large adhoc size (1280*720*3*10 + 1) here for mapping case
#define __UNOBTAINED_SIZE__  27648001

WinShm::WinShm() :
	hFile( NULL ),
	datap( NULL ),
	m_size( 0 )
{
}

bool WinShm::create( size_t size )
{
	hFile = NULL;
  TCHAR keyname[80];
	id = 0;
	while( !hFile ) {
		id++;
    _sntprintf_s( keyname, 80, TEXT("skypekit-surface-%i"), id );
		hFile = CreateFileMapping( INVALID_HANDLE_VALUE, 0, PAGE_READWRITE, 0, size, keyname );
		if ( hFile == NULL ) {
			// printf( "Failed to create mapping: %d\n", GetLastError() );
			return false;
		}
		if ( GetLastError() == ERROR_ALREADY_EXISTS ) {
			CloseHandle( hFile );
			hFile = NULL;
		}
	}
	datap = MapViewOfFile( hFile, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
	if ( !datap ) {
		// printf( "Failed to map file: %d\n", GetLastError() );
		CloseHandle( hFile );
		hFile = NULL;
		return false;
	}
	m_size=size;
	return true;
}

bool WinShm::destroy()
{
	unmap();
	return true;
}

bool WinShm::map( int key )
{
  TCHAR keyname[80];
  _sntprintf_s( keyname, 80, TEXT("skypekit-surface-%i"), key );
	hFile = OpenFileMapping( FILE_MAP_READ | FILE_MAP_WRITE, FALSE, keyname );
	if ( !hFile )
		return false;
	datap = MapViewOfFile( hFile, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0 );
	if ( !datap ) {
		CloseHandle( hFile );
		hFile = NULL;
		m_size = 0;
		return false;
	}
	m_size = __UNOBTAINED_SIZE__;
	return true;
}

bool WinShm::unmap()
{
	if ( datap )
		UnmapViewOfFile( datap );
	if ( hFile )
		CloseHandle( hFile );
	datap = NULL;
	hFile = NULL;
	m_size = 0;
	return true;
}

int WinShm::key()
{
	return id;
}

void *WinShm::data()
{
	return datap;
}

const char *WinShm::getName()
{
	return "Win";
}

size_t WinShm::getSize()
{
	return m_size;
}

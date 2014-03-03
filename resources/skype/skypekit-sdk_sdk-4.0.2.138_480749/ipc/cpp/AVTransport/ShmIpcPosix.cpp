/*
 * ShmImpl.cpp
 *
 *  Created on: May 11, 2010
 *      Author: lauri
 */
#include "ShmTransport.hpp"
//#define SID_LOG_MASK 0x1f
//#define SID_DBG_TIMESTAMPS 1
#include "SidDebugging.hpp"

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/mman.h>

namespace Sid {

class MmapShm: public Shm {
	unsigned char *m_Shmemory;
	int m_Fd;
	int m_Create;
	int m_Size;
	char m_FileName[255];
public:
	MmapShm();
	~MmapShm();
	bool Init(const char *fileName, int size, int serverMode);
	unsigned char * GetMemPtr() {
		return m_Shmemory;
	}
	void Uninit();
};

class PosixSemaphore: public Semaphore {
	sem_t *m_Sem;
	bool m_Create;
	char m_FileName[255];
	MmapShm m_Shm;
public:
	PosixSemaphore();
	~PosixSemaphore();

	bool Init(const char *fileName, const char *fileSuffix, bool create,
			int initval);
	void Uninit();

	bool Wait();
	bool Wait(int timeout_ms);
	bool Signal();
};

Semaphore *GetSem() {
	return new PosixSemaphore();
}

Shm *GetShm() {
	return new MmapShm();
}

PosixSemaphore::PosixSemaphore() :
	Semaphore(),
	m_Sem(SEM_FAILED),
	m_Create(false) {

}
PosixSemaphore::~PosixSemaphore() {
	Uninit();
}

bool PosixSemaphore::Init(const char *fileName, const char *fileSuffix,
		bool create, int initval) {
	m_Create = create;
	snprintf(m_FileName, sizeof(m_FileName), "%s%s", fileName, fileSuffix);

	SID_VDBG("PosixSemaphore::Init sem %s, create: %d, initval: %d", m_FileName, create, initval);

	/* android libc only supports sem_init, not sem_open. So we need to create small shm where we can place semaphore*/
	if (!m_Shm.Init(m_FileName, sizeof(sem_t), create)) {
		SID_ERROR("m_Shm.Init: %s: failed!, error %d %s", m_FileName, errno, strerror(errno));
		Uninit();
		return false;
	}
	m_Sem = (sem_t*)m_Shm.GetMemPtr();

	if (create) {
		if (sem_init(m_Sem, 0, initval) == -1) {
			m_Sem = SEM_FAILED;
			SID_ERROR("sem_init: %s: failed!, error %d %s", m_FileName, errno, strerror(errno));
			Uninit();
			return false;
		}
	}

	return true;
}

void PosixSemaphore::Uninit() {
	SID_DBG("PosixSemaphore::Uninit: sem %s", m_FileName);
	if (m_Sem != SEM_FAILED) {
		if (m_Create) {
			if (sem_destroy(m_Sem) < 0)
				SID_ERROR("sem_destroy: %s failed!, error %d %s", m_FileName, errno, strerror(errno));
		}
		m_Sem = SEM_FAILED;
	}
	m_Shm.Uninit();
}

bool PosixSemaphore::Wait() {
	SID_VDBG("PosixSemaphore::Wait() %s", m_FileName);

	while (sem_wait(m_Sem) == -1) {
		if (errno != EINTR) {
			SID_ERROR("sem_wait %s: failed, errno: %d (%s)", m_FileName, errno, strerror(errno));
			return false;
		}
	}
	return true;
}

bool PosixSemaphore::Wait(int timeout_ms) {
	SID_VDBG("PosixSemaphore::Wait(%d) %s", timeout_ms, m_FileName);
	if (timeout_ms < 0)
		return Wait();

	struct timespec ts;
	ts.tv_sec = timeout_ms / 1000;
	ts.tv_nsec = (timeout_ms % 1000) * 1000;
	SID_DBG("timedop: ms: %d sec: %ld, nsec: %ld", timeout_ms, ts.tv_sec, ts.tv_nsec);
	while (sem_timedwait(m_Sem, &ts) == -1) {
		if (errno != EINTR) {
			SID_ERROR("sem_timedwait %s: failed, errno: %d (%s)", m_FileName, errno, strerror(errno));
			return false;
		}
	}
	return true;
}

bool PosixSemaphore::Signal() {
	SID_VDBG("PosixSemaphore::Signal() %s", m_FileName);
	if (sem_post(m_Sem) == -1) {
		SID_ERROR("sem_post %s: failed, errno: %d (%s)", m_FileName, errno, strerror(errno));
	}
	return true;
}

MmapShm::MmapShm() :
	Shm(), m_Shmemory(NULL), m_Fd(-1), m_Size(0), m_Create(false) {
}

MmapShm::~MmapShm() {
	Uninit();
}


bool MmapShm::Init(const char *fileName, int size, int create) {
	SID_DBG("Shm::Init: %s, size: %d, create: %d", fileName, size, create);
	m_Create = create;
	m_Size = size;
	strncpy(m_FileName, fileName, sizeof(m_FileName));
	int m_Fd = open(m_FileName, (create ? O_CREAT | O_RDWR  : O_RDWR), 0600);
	if (m_Fd < 0) {
		SID_ERROR( "ShmChannel: open failed: file %s, errno: %d, strerror(errno): %s", m_FileName, errno, strerror(errno));
		Uninit();
		return false;
	}

	if (create) {
		if (ftruncate(m_Fd, size) == -1) {
			SID_ERROR( "ShmChannel: ftruncate failed: file %s, errno: %d, strerror(errno): %s", m_FileName, errno, strerror(errno));
			Uninit();
			return false;
		}
	}

	if ((int)(m_Shmemory = (unsigned char *)mmap(0, size, PROT_READ|PROT_WRITE, MAP_SHARED, m_Fd, 0)) == -1) {
		SID_ERROR( "ShmChannel: mmap failed: file %s, errno: %d, strerror(errno): %s", m_FileName, errno, strerror(errno));
		Uninit();
		return false;
	}
	return true;
}

void MmapShm::Uninit() {
	if (m_Shmemory) {
		if (munmap(m_Shmemory, m_Size)) {
			SID_ERROR("Failed to detach shared memory: file %s, errno: %d strerror(errno): %s", m_FileName, errno, strerror(errno));
		}
		m_Shmemory = NULL;
	}

	if (m_Fd != -1) {
		close(m_Fd);
		m_Fd = -1;
	}
}

}

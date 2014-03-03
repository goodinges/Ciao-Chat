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

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>

namespace Sid {

class SysVSemaphore: public Semaphore {
	int m_SemId;
	int m_Fd;
	bool m_Create;
	char m_FileName[255];
	key_t m_Key;

public:
	SysVSemaphore();
	~SysVSemaphore();

	bool Init(const char *fileName, const char *fileSuffix, bool create,
			int initval);
	void Uninit();

	bool Wait();
	bool Wait(int timeout_ms);
	bool Signal();
};

class SysVShm: public Shm {
	int m_Shmid;
	unsigned char *m_Shmemory;
	int m_Fd;
	int m_Create;
public:
	SysVShm();
	~SysVShm();
	bool Init(const char *fileName, int size, int serverMode);
	unsigned char * GetMemPtr() {
		return m_Shmemory;
	}
	void Uninit();
};

Semaphore *GetSem() {
	return new SysVSemaphore();
}

Shm *GetShm() {
	return new SysVShm();
}

SysVSemaphore::SysVSemaphore() :
	Semaphore(),
	m_SemId(-1),
	m_Fd(-1),
	m_Create(false),
	m_Key(0) {

}
SysVSemaphore::~SysVSemaphore() {
	Uninit();
}

bool SysVSemaphore::Init(const char *fileName, const char *fileSuffix,
		bool create, int initval) {
	m_Create = create;
	snprintf(m_FileName, sizeof(m_FileName), "%s%s", fileName, fileSuffix);

	SID_VDBG("Init sem %s, create: %d, initval: %d", m_FileName, create, initval);

	int m_Fd = open(m_FileName, (create ? O_WRONLY | O_CREAT : O_WRONLY),
			0600);
	if (m_Fd == -1) {
		SID_ERROR("sem: %s: File open failed!, error %d %s", m_FileName, errno, strerror(errno));
		Uninit();
		return false;
	}

	key_t m_Key = ftok(m_FileName, 0);
	if (m_Key == -1) {
		SID_ERROR("sem: %s: ftok failed!, error %d %s", m_FileName, errno, strerror(errno));
		Uninit();
		return false;
	}

	if ((m_SemId = semget(m_Key, 1, IPC_CREAT | 0600)) == -1) {
		SID_DBG("sem: %s: Failed to create a semaphore: errno %d (%s)", m_FileName, errno, strerror(errno));
		Uninit();
		return false;
	}
	if (create) {
		short sarray[1];
		sarray[0] = initval;
		if (semctl(m_SemId, 1, SETALL, sarray) == -1) {
			SID_ERROR("sem: %s: semctl(m_SemId, 0, SETVAL, se) failed, errno: %d (%s)", m_FileName, errno, strerror(errno));
			Uninit();
			return false;
		}
	}
	return true;
}

void SysVSemaphore::Uninit() {
	SID_DBG("SysVSemaphore::Uninit: sem %s: id: %d, fd: %d", m_FileName, m_SemId, m_Fd);
	if (m_SemId != -1) {
		if (m_Create) {
			SID_DBG("SysVSemaphore::Uninit remove sem %s", m_FileName);
			if (semctl(m_SemId, 1, IPC_RMID) == -1) {
				SID_ERROR("sem %s: semctl(semid, 1, IPC_RMID) failed, errno: %d (%s)", m_FileName, errno, strerror(errno));
			}
		}
		m_SemId = -1;
	}
	if (m_Fd != -1) {
		close(m_Fd);
		m_Fd = -1;
	}
}

bool SysVSemaphore::Wait() {
	struct sembuf sem_op[1];
	sem_op[0].sem_num = 0;
	sem_op[0].sem_op = -1;
	sem_op[0].sem_flg = 0;
	while (semop(m_SemId, sem_op, 1) == -1) {
		if (errno != EINTR) {
			SID_ERROR("sem %s: semop(m_SemId, &sem_op, 1) failed, errno: %d (%s)", m_FileName, errno, strerror(errno));
			return false;
		}
	}
	/*
	 int val;
	 if ((val = semctl(m_SemId, 0, GETVAL)) == -1) {
	 SID_ERROR("semctl(semid, 0, IPC_RMID) failed, errno: %d (%s)", errno, strerror(errno));
	 }
	 SID_VDBG("Semaphore::Wait val == %d, (%s %d)", val, m_FileSuffix, m_Key);
	 */
	return true;
}

bool SysVSemaphore::Wait(int timeout_ms) {
	if (timeout_ms < 0)
		return Wait();

	struct sembuf sem_op[1];
	sem_op[0].sem_num = 0;
	sem_op[0].sem_op = -1;
	sem_op[0].sem_flg = 0;

	struct timespec ts;
	ts.tv_sec = timeout_ms / 1000;
	ts.tv_nsec = (timeout_ms % 1000) * 1000;
	SID_DBG("timedop: ms: %d sec: %ld, nsec: %ld", timeout_ms, ts.tv_sec, ts.tv_nsec);
	while (semtimedop(m_SemId, sem_op, 1, &ts) == -1) {
	 	if (errno != EINTR) {
			SID_ERROR("sem %s: semtimedop(m_SemId, &sem_op, 1) failed, errno: %d (%s)", m_FileName, errno, strerror(errno));
			return false;
		}
	}
	return true;
}

bool SysVSemaphore::Signal() {
	struct sembuf sem_op[1];
	sem_op[0].sem_num = 0;
	sem_op[0].sem_op = 1;
	sem_op[0].sem_flg = 0;
	while (semop(m_SemId, sem_op, 1) == -1) {
		if (errno == 34)
			return true;
		if (errno != EINTR)  {
			SID_ERROR("sem %s: semop(m_SemId, &sem_op, 1) failed, errno: %d (%s)", m_FileName, errno, strerror(errno));
			return false;
		}
	}
	return true;
}

SysVShm::SysVShm() :
	Shm(), m_Shmid(-1), m_Shmemory(NULL), m_Fd(-1), m_Create(false) {
}

SysVShm::~SysVShm() {
	Uninit();
}

bool SysVShm::Init(const char *fileName, int size, int create) {
	SID_DBG("Shm::Init: %s, size: %d, create: %d", fileName, size, create);
	m_Create = create;
	int m_Fd = open(fileName, (create ? O_WRONLY | O_CREAT : O_WRONLY), 0600);
	if (m_Fd == -1) {
		SID_ERROR("File %s open failed!, error %d %s", fileName, errno, strerror(errno));
		Uninit();
		return false;
	}

	key_t key = ftok(fileName, 0);
	if (key == -1) {
		SID_ERROR("ftok %s failed!, error %d %s", fileName, errno, strerror(errno));
		Uninit();
		return false;
	}

	if ((m_Shmid = shmget(key, size, (create ? IPC_CREAT | 0600 : 0))) == -1) {
		SID_ERROR( "ShmChannel: shmget failed: file %s, key: 0x%x, res: %d, errno: %d, strerror(errno): %s", fileName, key, m_Shmid, errno, strerror(errno));
		Uninit();
		return false;
	}

	if ((m_Shmemory = (unsigned char *) shmat(m_Shmid, NULL, 0))
			== (unsigned char *) -1) {
		SID_ERROR( "Failed to attach shared memory: %s", strerror( errno ) );
		Uninit();
		return false;
	}
	return true;
}

void SysVShm::Uninit() {
	if (m_Shmemory) {
		if (shmdt(m_Shmemory)) {
			SID_ERROR("Failed to detach shared memory: %d (%s)", errno, strerror(errno));
		}
		m_Shmemory = NULL;
	}

	if (m_Shmid != -1) {
		if (m_Create) {
			struct shmid_ds shmid_struct;
			if (shmctl(m_Shmid, IPC_RMID, &shmid_struct) == -1) {
				SID_ERROR( "shmctl(m_Shmid, IPC_RMID, &shmid_struct) failed : %d (%s)", errno, strerror(errno));
			}
		}
		m_Shmid = -1;
	}
	if (m_Fd != -1) {
		close(m_Fd);
		m_Fd = -1;
	}
}

}

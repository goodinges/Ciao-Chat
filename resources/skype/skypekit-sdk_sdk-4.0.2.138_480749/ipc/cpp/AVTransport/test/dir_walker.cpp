#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

volatile char g_c;

int g_optMappedReads;
const char* g_startLocation = "/";
bool g_dontFollowSymlinks = false;

static bool walk(const char* path)
{
	
	DIR* dp = opendir(path);
	if (!dp)
	{
		return true;
	}
	struct dirent* de = 0;
	while ((de = readdir(dp)) != 0)
	{
		if (strcmp(de->d_name, ".") == 0 ||
			strcmp(de->d_name, "..") == 0)
		{
			continue;
		}
		
		printf("%s\n", path);
		char* sp = (char*)malloc(strlen(path) + strlen(de->d_name) + 2);
		if (!sp)
		{
			printf("Error: out of memory\n");
			return false;
		}
		if (path[strlen(path) - 1] == '/')
		{
			sprintf(sp, "%s%s", path, de->d_name);
		} 
		else
		{
			sprintf(sp, "%s/%s", path, de->d_name);
		}
		
		// If it's a file, map it, read blindly. And don't recurse. Otherwise recurse.
		struct stat sb;
		int (*stf)(const char*, struct stat*) = stat;
		if (g_dontFollowSymlinks)
		{
			stf = lstat;
		}
		if (-1 != lstat(sp, &sb))
		{
			int fd = -1;
			if (sb.st_mode & S_IFDIR)
			{
				walk(sp);
			}
			else if (sb.st_mode & S_IFREG && g_optMappedReads)
			{
				if ((fd = open(sp, O_RDONLY)) != -1)
				{
					char* fp = (char*)mmap(0, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
					if (fp != (char*)MAP_FAILED)
					{
						// Randomly access some bytes in it.
						unsigned cnt = 22;
						while (cnt --)
						{
							volatile char g_c = *(fp + (rand() % sb.st_size));
						}
						
						munmap(fp, sb.st_size);
					}
					else
					{
						perror("mmap");
					}
					close(fd);
				}
				else
				{
					perror("open");
				}
			}
		}
		else
		{
			perror("stat");
		}
		
		
		
		free(sp);
		sp = 0;
	}
	closedir(dp);
	dp = 0;
	return true;
}


static void usage(const char* pname)
{
	printf("usage: %s [-r <mapped reads per file>] [-L] [-s <start location>]\n", pname);
	exit(EXIT_FAILURE);
}


int main(int argc, char** argv)
{
	const char* pname = argv[0];
	argc --, argv ++;
	while (argc != 0)
	{
		if (strcmp(argv[0], "-L") == 0)
		{
			argc --, argv ++;
			g_dontFollowSymlinks = true;
		}
		else if (strcmp(argv[0], "-s") == 0)
		{
			argc --, argv ++;
			if (argc == 0)
			{
				usage(pname);
			}
			g_startLocation = argv[0];
			argc --, argv ++;
		}
		else if (strcmp(argv[0], "-r") == 0)
		{
			argc --, argv ++;
			if (argc == 0)
			{
				usage(pname);
			}
			g_optMappedReads = atoi(argv[0]);
			argc --, argv ++;
		}
		else
		{
			usage(pname);
		}
	}

	while (walk(g_startLocation))
	{
	}
	return EXIT_FAILURE;
}

/**
 * @file lib_mmap.c
 * @author Kishore
 * @date October 4, 2015, 2:59 PM
 * 
 * @brief 
 * 
 * 
 */

#include "headers/init.h"
#include "headers/core_ansi.h"
#include "headers/core_file.h"
/**
 * Maps the file to memory and returns the pointer to the memory.
 * @param file : valid path of file.
 * @returns pointer to the data.
 * 
 * No need of error checking.
 */
void* file_map(char *file)
{
	int fd = open(file, O_RDWR,  S_IRUSR | S_IWUSR);
	assert(fd!=-1);

	struct stat sb;
	assert(fstat(fd, &sb)!=-1);

	void *addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	assert(addr!=MAP_FAILED);

	return addr;
}


/**
 * Creates file specified in `file` with `prealloc_memory` size pre allocated.
 * @param file : valid path where the file should be created.
 * @param prealloc_memory : The size of file that should be preallocated.
 * @returns file descriptor of the created file.
 * 
 * This function throws error if file already exist, or do not have specified
 * permissions to create file there.
 */
int file_create(char *file, int prealloc_memory)
{
	int fd;
	assert(!fileio_shouldexist(file));
	
	fd = open(file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
	assert(fd!=-1);

	int i=prealloc_memory;
	char prealloc[1024];
	
	while (i!=0){
		int w = (i>1024)?1024:i;
		int written = write(fd, prealloc, w);
		assert(written==(w));
		i -= w;
	}
	return fd;
}

/**
 * Returns the file size of the file specified in `file`.
 * @param file : Valid file path.
 */
int file_size(char *file)
{
	int fd = file_open(file);
	struct stat sb;
	assert(fstat(fd, &sb)!=-1);
	
	return sb.st_size;
}

/**
 * Checks if file specified in `file` exists or not.
 * @param file : valid file path.
 * @returns true if exists and false if do not exist.
 */
int file_exist(char *file)
{
	int fd = open(file, O_RDWR);
	close(fd);
	if (fd==-1)
	{
		return 0;
	}
	return 1;
}


/**
 * Opens 
 * @param file
 * @return 
 */
int file_open(char *file)
{
	int fd = open(file, O_RDWR);
	if (fd!=-1)
		return fd;
	file_error(file);
	return -1;
}


/**
 * Prints the error occurred during opening the file.
 * @param file : Just for referencing.
 */
void file_error(char *file)
{
	if (errno==EACCES)
		ansi_error("File %s has no permission to open in read or write mode.\n", file);
	if (errno==ENOENT)
		ansi_error("File %s do not exist.\n", file);
}
// argv[1] = name of the source file


#include <windows.h>						// for using winAPI functions
#include <stdio.h>							// for using std I/O functions
#include <stdlib.h>							// for converting user input string to numeric

#define MAX_NAME 256						// max name of a file
#define MAX_PATH 512						// path of the source or destination file
#define MAPPED_OBJ_NAME "my_mapped_obj"		// must be defined in all process which wish get access to that obj

int main(int argc, char *argv[])
{
	/*//////////////////////////////////////////////////////////////////////////////////////////////////////////////
										
	1. creating a mapped object on the RAM, using the file handle, and this objrepresents the src file on the HD
	2. note: the responsibily to close the mapped object is for the process that uses it
	3. note: MAPPED_OBJ_NAME cannot be changed without changing this value in other processes 
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////*/


	int fh_src;								// file on the dist - handle to the source file (generated by python script) which is on the disk
	int mapped_obj_src = 0;					// obj on the RAM - fh_src mapped to the disk and creates this obj
	char mapped_obj_name[MAX_NAME] = { 0 };


	char path_src[MAX_PATH] = { 0 };

	strcpy(mapped_obj_name, MAPPED_OBJ_NAME);
	strcpy(path_src, argv[1]);				// get the src path argument

	// open the src file and get the handle to the file
	fh_src = CreateFile(path_src, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	// check valid handle
	if (fh_src == INVALID_HANDLE_VALUE)
	{
		printf("%s\n", path_src);
		printf("cannot open the source file\n");
		exit(1);
	}

	// create a file mapping object (on the RAM) fromr the source file (from HD)
	// note: because we will share this object, we must share it's name, so we named it
	mapped_obj_src = CreateFileMappingA(fh_src, NULL, PAGE_READWRITE, 0, 0, mapped_obj_name);

	// check valid obj
	if (mapped_obj_src == NULL)
	{
		printf("cannot map the source file\n");
		exit(1);
	}

	// close src file
	CloseHandle(fh_src);
}
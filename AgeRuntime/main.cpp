#include <AgeAPI/AgeAPI.hpp>
#include <Windows.h>
int main()
{
	// print cwd with windows api
	char cwd[MAX_PATH];
	GetCurrentDirectoryA(MAX_PATH, cwd);
	printf("cwd: %s\n", cwd);
}
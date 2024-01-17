#include <stdio.h>
#include <windows.h>

DWORD WINAPI ThreadProc(LPVOID Arg) {
	printf("%s\n", Arg);

	return 0;
}

char Arg[] = "Hello thread!";

int main() {
	printf("Hello world!\n");
	
    HANDLE ThreadHandle = CreateThread(NULL, 0, ThreadProc, Arg, 0, NULL);

    Sleep(1000);

    return 0;
}

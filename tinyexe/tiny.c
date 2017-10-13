
typedef unsigned int DWORD;
#define NULL (0)
#define _In_
#define _In_opt_
#define _Out_
#define WINAPI __stdcall
extern void WINAPI ExitProcess(_In_ DWORD uExitCode);
#pragma comment(lib, "kernel32")

void __stdcall WinMain()
{
  ExitProcess(0);
}
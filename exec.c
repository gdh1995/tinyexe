// author: gdh1995@qq.com

typedef void *PVOID;
typedef PVOID HANDLE;
typedef HANDLE HINSTANCE, HWND, HMODULE;
typedef unsigned short tchar;
typedef unsigned int DWORD;

#define NULL (0)
#define _In_
#define _In_opt_
#define _Out_
#define WINAPI __stdcall

#pragma comment(lib, "kernel32")
#pragma comment(lib, "shell32")

extern HINSTANCE WINAPI ShellExecuteW(
  _In_opt_  HWND        hwnd,
  _In_opt_  const tchar *lpOperation,
  _In_      const tchar *lpFile,
  _In_opt_  const tchar *lpParameters,
  _In_opt_  const tchar *lpDirectory,
  _In_      int         nShowCmd
);
#define ShellExecute ShellExecuteW

extern tchar * WINAPI GetCommandLineW(void);
#define GetCommandLine GetCommandLineW

extern void WINAPI ExitProcess(_In_ DWORD uExitCode);

#define oper (NULL)
#define pwd (NULL)
#define showCmd (1)

#define SW_MINIMIZE (6)

void __stdcall MyMain() {
#ifdef SUDO
  #undef oper
  static const tchar oper[] = L"runas";
#endif

  tchar *p = GetCommandLine(), *cmd, *args;
#if defined CDEXEC
  #undef pwd
  tchar *pwd;
#endif
#ifdef START2
  #undef showCmd
  int showCmd = 1;
#endif

  // read path of self
  // note: ignore the pattern of '"""': not in file path
  if (*p == L'"') {
    while (*++p != L'"' && *p != L'\0') {}
    p++;
  }
  else {
    while (*++p != L' ' && *p != L'\0') {}
  }

#ifdef START2
  // read args of exec
  for (; ; ) {
    for (; *p == L' '; p++) {}
    if (*p != L'/' && *p != L'-') { break; }
    args = p;
    if (p[1] == L'-' && *p == L'-') {
      for (p += 2; *p == L' '; p++) {}
      break;
    }
    if ((*++args | (short)32) == L'm' && (*++args | (short)32) == L'i' && (*++args | (short)32) == L'n') {
      showCmd = SW_MINIMIZE;
    }
    else {
      break;
    }
    p = args + 1;
  }
#else
  for (; *p == L' '; p++) {}
#endif

  if (*p == L'\0') {
    ExitProcess(0);
    return;
  }

#ifdef CDEXEC
  // read pwd
  pwd = p;
  if (*p == L'"') {
    while (*++p != L'"' && *p != L'\0') {
      if (*p == L'/') { *p = L'\\'; }
    }
    cmd = p;
    p++;
    pwd++;
  }
  else {
    while (*++p != L' ' && *p != L'\0') {
      if (*p == L'/') { *p = L'\\'; }
    }
    cmd = p;
  }
  for (; *p == L' '; p++) {}
  if (*p == L'\0') {
    ExitProcess(0);
    return;
  }
  *cmd = L'\0';
#endif

  // read file to exec
  cmd = p;
#ifdef START2
  if (*p == L'"') {
    while (*++p != L'"' && *p != L'/' && *p != L'\\' && *p != L':' && *p != L'\0') {}
  }
  else {
    while (*++p != L'\0' && *p != L'/' && *p != L'\\' && *p != L' ' && *p != L':') {}
  }
  if (*p == L':' && p[1] != L'\0' && p[1] != L'\\') { // url
    if (*cmd == L'"') {
      while (*++p != L'"' && *p != L'\0') {}
      *p = L'\0';
      p++;
      cmd++;
    } else {
      while (*++p != L'\0' && *p != L' ') {}
    }
  }
  else {
    p = cmd;
#endif
    if (*p == L'"') {
      while (*++p != L'"' && *p != L'\0') {
        if (*p == L'/') { *p = L'\\'; }
      }
      *p = L'\0';
      p++;
      cmd++;
    }
    else {
      while (*++p != L' ' && *p != L'\0') {
        if (*p == L'/') { *p = L'\\'; }
      }
    }
#ifdef START2
  }
#endif
  args = *p ? (p + 1) : NULL;
  *p = L'\0';

  ShellExecute(NULL, oper, cmd, args, pwd, showCmd);
  ExitProcess(0);
}

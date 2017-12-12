// author: gdh1995@qq.com

#ifdef __cplusplus
#define CPP
#endif

typedef void *PVOID;
typedef PVOID HANDLE;
typedef HANDLE HINSTANCE, HWND, HMODULE;
typedef unsigned int DWORD;

#define MIN(x, y) (x) <= (y) ? (x) : (y)

#define NULL (0)
#define _In_
#define _In_opt_
#define _Out_
#define WINAPI __stdcall

#ifdef CPP
extern "C" {
#define tchar wchar_t
#else
typedef unsigned short tchar;
#endif
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

#define SW_MINIMIZE (6)

#define oper (NULL)
#define realcmd (cmd)
#define realargs (args)
#define pwd (NULL)
#define showCmd (1)

/*
#undef CDRUN
#define STATICRUN
//*/
//#define NO_INSERTED_ARGS

#ifdef CDRUN
#define STATICRUN
#endif

#ifdef STATICRUN
#define MAX_PATH (260)
#define BUFFER_LENGTH_IN_FILE (256)
#define MAX_EXEC_ARG_LENGTH (65536 / sizeof(tchar))
extern HMODULE WINAPI GetModuleHandleW(
  _In_opt_ const tchar* lpModuleName
);
extern DWORD WINAPI GetModuleFileNameW(
  _In_opt_ HMODULE hModule,
  _Out_    tchar*  lpFilename,
  _In_     DWORD   nSize
);

// inline
tchar *strcpy2(tchar *target, const tchar *src) {
  while (*src != L'\0') {
    *target++ = *src++;
  }
  *target = L'\0';
  return target;
}

//*
// inline
unsigned short *strcpyFromAsciToUnicode(unsigned short *dest, const unsigned char *src) {
  while (*src != L'\0') {
    *dest++ = *src++;
  }
  *dest = L'\0';
  return dest;
}
//*/

tchar *getBaseDir(tchar *const buffer) {
  HMODULE hModuleSelf = GetModuleHandleW(NULL);
  *buffer = L'$', buffer[1] = L'\0';
  tchar *p = buffer + 1;
  GetModuleFileNameW(hModuleSelf, p, MAX_PATH - 1);
  for (; *p != L'\0'; p++) {
    if (*p == L' ') { *buffer = L'"'; }
  }
  for (; p > buffer && *p != L'\\'; p--) {}
  if (p > buffer) {
    p++;
  }
  *p = L'\0';
  return p;
}

#endif
#define volatile
#ifdef STATICRUN
extern volatile const char _cmd_in_file[
#ifndef NO_INSERTED_ARGS
  2
#else
  1
#endif
][BUFFER_LENGTH_IN_FILE * 2];
  static tchar full_args[MAX_EXEC_ARG_LENGTH];
#endif
#ifdef SUDO
#undef oper
  extern const tchar oper[];
#endif
#if defined START2 || defined STATICRUN
#undef showCmd
#endif
#if defined START2 || defined STATICRUN
#undef oper
#endif

void __stdcall MyMain() {
  static tchar _config_exe_ucs[BUFFER_LENGTH_IN_FILE];
  static tchar exe_path[MIN(MAX_PATH * 2, BUFFER_LENGTH_IN_FILE * 2)];

  tchar *p, *cmd, *args;
#if defined CDEXEC || defined CDRUN
#undef pwd
  tchar *pwd;
#endif
#if defined START2 || defined STATICRUN
  int showCmd = *(int*)&_cmd_in_file[0][16];
#endif
#ifdef STATICRUN
#undef realcmd
#undef realargs
  const tchar *const oper = _cmd_in_file[0][16 + 4] != '\0' ? (const tchar *)&_cmd_in_file[0][16 + 4] : NULL;
  const tchar *const exe_to_run_const = (const tchar *)&_cmd_in_file[0][32];
  tchar *const exe_to_run = _config_exe_ucs;
#ifndef NO_INSERTED_ARGS
  const tchar *args_to_insert = (const tchar *)_cmd_in_file[1];
#else
#define args_to_insert ((const tchar *)(void*)NULL)
#endif
  const tchar *realcmd, *realargs;
  tchar *end_of_base_dir = NULL;
  HMODULE hModuleSelf = NULL;

  if (exe_to_run_const[0] == L'\0') {
    ExitProcess(0);
    return;
  }
#ifndef NO_INSERTED_ARGS
  if (args_to_insert != NULL && *((const char*)args_to_insert) != '\0') {
    if (((const char*)args_to_insert)[1] != '\0') {
      strcpyFromAsciToUnicode((unsigned short *)_config_exe_ucs, (const unsigned char*)args_to_insert);
      args_to_insert = _config_exe_ucs;
    }
    // args_to_insert to full_args;
    realargs = p = full_args;
    while (*args_to_insert != L'\0') {
      if (*args_to_insert != L'~') {
        *p++ = *args_to_insert++;
        continue;
      }
      if (end_of_base_dir == NULL) {
        end_of_base_dir = getBaseDir(exe_path);
      }
      p = strcpy2(p, *exe_path == L'"' ? exe_path : exe_path + 1);
      args_to_insert += args_to_insert[1] == L'\\' ? 2 : 1;
      while (*args_to_insert != L' ' && *args_to_insert != L'\0') {
        *p++ = *args_to_insert++;
      }
      if (*exe_path == L'"') {
        *p++ = L'"';
      }
    }
    *p = L'\0';
  }
  else {
    realargs = NULL;
  }
#endif
  if (((const char*)exe_to_run_const)[1] != '\0') {
    strcpyFromAsciToUnicode(exe_to_run, (const unsigned char*)exe_to_run_const);
  }
  else {
    strcpy2(exe_to_run, (const tchar *)exe_to_run_const);
  }
  for (p = exe_to_run; *p != L'\0'; p++) {
    if (*p == L'/') { *p = L'\\'; }
  }
  if (*exe_to_run == L'$') {
    realcmd = exe_to_run + 1;
  }
  else if (exe_to_run[1] == L':' && (*exe_to_run | (short)32) >= L'a' && (*exe_to_run | (short)32) <= L'z') {
    realcmd = exe_to_run;
  }
  else if (*exe_to_run == L'\\' && exe_to_run[1] == L'\\') {
    realcmd = exe_to_run;
  }
  else {
    if (end_of_base_dir == NULL) {
      end_of_base_dir = getBaseDir(exe_path);
    }
    realcmd = p = exe_path + 1;
    if (*exe_to_run != L'\\') {
      p = end_of_base_dir;
    }
    else if (p[1] == L':') {
      p += 2;
    }
    strcpy2(p, exe_to_run);
  }
#endif

  p = GetCommandLine();
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
#ifdef STATICRUN
    ShellExecute(NULL, oper, realcmd, realargs, NULL, showCmd);
#endif
    ExitProcess(0);
    return;
  }

#ifdef CDRUN
  if (*p == L'-' && p[1] == L'-' && p[2] == L'c' && p[3] == L'd' && p[4] == L'=') {
    p += 5;
#endif
#if defined CDEXEC || defined CDRUN
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
    *cmd = L'\0';
    if (*p == L'\0') {
#ifdef STATICRUN
      ShellExecute(NULL, oper, realcmd, realargs, pwd, showCmd);
#endif
      ExitProcess(0);
      return;
    }
#endif
#ifdef CDRUN
  }
  else {
    pwd = NULL;
  }
#endif

  // read file to exec
#ifndef STATICRUN
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
    }
    else {
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
  args = *p != L'\0' && p[1] != L'\0' ? p + 1 : NULL;
  *p = L'\0';
#else
  args = p;
#endif

#ifdef STATICRUN
#ifndef NO_INSERTED_ARGS
  if (realargs == NULL) {
#endif
    realargs = args;
#ifndef NO_INSERTED_ARGS
  }
  else {
    p = strcpy2(full_args, args_to_insert);
    *p++ = L' ';
    strcpy2(p, args);
    realargs = full_args;
  }
#endif
#endif
  ShellExecute(NULL, oper, realcmd, realargs, pwd, showCmd);
  ExitProcess(0);
}

#ifdef CPP
}
#endif
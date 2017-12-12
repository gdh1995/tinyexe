#ifdef CDRUN
#define STATICRUN
#endif

typedef unsigned short tchar;

#ifdef STATICRUN
#define MAX_PATH (260)
#define BUFFER_LENGTH_IN_FILE (256)
#define MAX_EXEC_ARG_LENGTH (65536 / sizeof(tchar))
#endif

#ifdef STATICRUN
extern const char _cmd_in_file[][BUFFER_LENGTH_IN_FILE * 2] = {
  "Show@I32" " Op@WSTR"
  "\x06\0\0\0" "\0\0r\0u\0n\0a\0s\0"
  "usr\\bin\\env.exe"
#ifndef NO_INSERTED_ARGS
  , "HOME=/home/gdh1995 ~\\usr\\bin\\_mintty.exe -"
#else
#define args_to_insert ((const tchar *)(void*)NULL)
#endif
};
#endif
#ifdef SUDO
#undef oper
extern const tchar oper[] = L"runas";
#endif

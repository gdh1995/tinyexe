#ifdef CDRUN
#define STATICRUN
#endif

typedef unsigned short tchar;

#ifdef STATICRUN
#define MAX_PATH (260)
#define BUFFER_LENGTH_IN_FILE (256)
#define MAX_EXEC_ARG_LENGTH (65536 / sizeof(tchar))
#endif

#if defined START2 || defined STATICRUN
#undef showCmd
extern volatile const int _showcmd_in_file[4] = { 'wohS', ':dmC', ' :4/' , 6 };
#endif
#ifdef STATICRUN
extern volatile const tchar _cmd_in_file[][BUFFER_LENGTH_IN_FILE] = {
  L"usr\\bin\\env.exe"
#ifndef NO_INSERTED_ARGS
  , L"HOME=/home/gdh1995 ~\\usr\\bin\\_mintty.exe -"
#else
#define args_to_insert ((const tchar *)(void*)NULL)
#endif
};
static tchar full_args[MAX_EXEC_ARG_LENGTH] = L"";
#endif
#ifdef SUDO
#undef oper
extern const tchar oper[] = L"runas";
#endif

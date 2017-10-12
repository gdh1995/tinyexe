// exec.cpp : 定义控制台应用程序的入口点。
//

#include <cwchar>
#include <cstring>
#include <string>

using namespace std;

#pragma comment(lib, "shell32")
extern "C" wchar_t * __stdcall GetCommandLineW(void);
#define GetCommandLine GetCommandLineW

static const char cmd[512] = "cmd.exe /c";
static const char garg[16] = "x\0# global args";

int main()
{
	wchar_t *p = GetCommandLineW();

	// read path of self
	if (*p == L'"') {
		while (*++p != L'"') {}
		p++;
	}
	else {
		while (*++p != L' ' && *p != L'\0') {}
	}
	if (*p == L'\0') {
		return 0;
	}

	bool need_print = strchr(garg, 'x') != NULL;
	// read args of exec
	for (;;) {
		for (; *p == L' '; p++) {}
		if (*p != L'-' && *p != L'+') { break; }
		wchar_t *args = p + 1;
		bool enable = *p == L'-';
		if (*args == L'-' && enable) {
			p = args;
			while (*++p == L' ') {}
			break;
		}
		if (*args == L'x') {
			need_print = enable;
		}
		else {
			break;
		}
		p = args + 1;
	}
	if (*p == L'\0') {
		return 0;
	}
	p--; // unshift a space

#ifdef _DEBUG
	need_print = true;
	printf("%s", "[DEBUG] ");
#endif
	if (need_print) {
		_putws(p + 1);
	}

	bool is_unicode = cmd[1] == '\0';
	wstring s(is_unicode ? (const wchar_t *)cmd : L"");
	int len1 = (int)(is_unicode ? wcslen((const wchar_t *)cmd) : strlen(cmd)), len2 = (int)(wcslen(p));
	if (!is_unicode) {
		s.reserve(len1 + len2);
		s.resize(len1);
		for (int i = 0; cmd[i] != '\0'; i++) {
			s[i] = cmd[i];
		}
	}
	s += p;
	int ret = _wsystem(s.c_str());
	return ret;
}

